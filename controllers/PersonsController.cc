#include "PersonsController.h"
#include <memory>
#include <utility>
#include <vector>
#include <regex>

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Person fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
        auto json = *jsonPtr;
        if (json["department_id"]) json["department_id"] = std::stoi(json["department_id"].asString());
        if (json["manager_id"]) json["manager_id"] = std::stoi(json["manager_id"].asString());
        if (json["job_id"]) json["job_id"] = std::stoi(json["job_id"].asString());
        auto person = Person(json);
        return person;
    }
}  // namespace drogon

void PersonsController::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const {
    LOG_DEBUG << "get";
    auto sort_field = req->getOptionalParameter<std::string>("sort_field").value_or("id");
    auto sort_order = req->getOptionalParameter<std::string>("sort_order").value_or("asc");
    auto limit = req->getOptionalParameter<int>("limit").value_or(25);
    auto offset = req->getOptionalParameter<int>("offset").value_or(0);

    try {
        auto dbClientPtr = drogon::app().getDbClient();

        const char *sql = "select person.*, \n\
                           job.title as job_title, \n\
                           department.name as department_name, \n\
                           concat(manager.first_name, ' ', manager.last_name) as manager_full_name \n\
                           from person \n\
                           join job on person.job_id =job.id \n\
                           join department on person.department_id=department.id \n\
                           join person as manager on person.manager_id = manager.id \n\
                           order by $sort_field $sort_order \n\
                           limit $1 offset $2;";

        // hack workaroun
        auto sql_sub = std::regex_replace(sql, std::regex("\\$sort_field"), sort_field);
        sql_sub = std::regex_replace(sql_sub, std::regex("\\$sort_order"), sort_order);

        auto f = dbClientPtr->execSqlAsyncFuture(sql_sub, std::to_string(limit), std::to_string(offset));
        auto result = f.get(); // Block until we get the result or catch the exception;
        if (result.empty()) {
            Json::Value ret{};
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        Json::Value ret{};
        for (auto row : result) {
            PersonInfo personInfo{row};
            PersonDetails personDetails{personInfo};
            ret.append(personDetails.toJson());
        }

        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::getOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int personId) const {
    LOG_DEBUG << "getOne personId: "<< personId;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    const char *sql = "select person.*, \n\
                       job.title as job_title, \n\
                       department.name as department_name, \n\
                       concat(manager.first_name, ' ', manager.last_name) as manager_full_name \n\
                       from person \n\
                       join job on person.job_id =job.id \n\
                       join department on person.department_id=department.id \n\
                       join person as manager on person.manager_id = manager.id \n\
                       where person.id = $1";

    *dbClientPtr << std::string(sql)
                 << personId
                 >> [callbackPtr](const Result &result)
                   {
                      if (result.empty()) {
                          Json::Value ret{};
                          ret["error"] = "resource not found";
                          auto resp = HttpResponse::newHttpJsonResponse(ret);
                          resp->setStatusCode(HttpStatusCode::k404NotFound);
                        (*callbackPtr)(resp);
                      }

                      auto row = result[0];
                      PersonInfo personInfo{row};
                      PersonDetails personDetails{personInfo};

                      Json::Value ret = personDetails.toJson();
                      auto resp = HttpResponse::newHttpJsonResponse(ret);
                      resp->setStatusCode(HttpStatusCode::k200OK);
                      (*callbackPtr)(resp);
                   }
                 >> [callbackPtr](const DrogonDbException &e)
                   {
                      LOG_ERROR << e.base().what();
                      Json::Value ret{};
                      ret["error"] = "database error";
                      auto resp = HttpResponse::newHttpJsonResponse(ret);
                      resp->setStatusCode(HttpStatusCode::k500InternalServerError);
                      (*callbackPtr)(resp);
                   };

    }

// void PersonsController::getOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int personId) const {
//     LOG_DEBUG << "getOne personId: "<< personId;
//     try {
//         auto dbClientPtr = drogon::app().getDbClient();

//         const char *sql = "select person.*, \n\
//                            job.title as job_title, \n\
//                            department.name as department_name, \n\
//                            concat(manager.first_name, ' ', manager.last_name) as manager_full_name \n\
//                            from person \n\
//                            join job on person.job_id =job.id \n\
//                            join department on person.department_id=department.id \n\
//                            join person as manager on person.manager_id = manager.id \n\
//                            where person.id = $1";

//         auto f = dbClientPtr->execSqlAsyncFuture(sql, personId);

//         auto result = f.get(); // Block until we get the result or catch the exception;
//         if (result.empty()) {
//             Json::Value ret{};
//             ret["error"] = "resource not found";
//             auto resp = HttpResponse::newHttpJsonResponse(ret);
//             resp->setStatusCode(HttpStatusCode::k404NotFound);
//             callback(resp);
//         }

//         auto row = result[0];
//         PersonInfo personInfo{row};
//         PersonDetails personDetails{personInfo};

//         Json::Value ret = personDetails.toJson();
//         auto resp = HttpResponse::newHttpJsonResponse(ret);
//         resp->setStatusCode(HttpStatusCode::k200OK);
//         callback(resp);
//     } catch (const DrogonDbException & e) {
//         LOG_ERROR << e.base().what();
//         Json::Value ret{};
//         ret["error"] = "database error";
//         auto resp = HttpResponse::newHttpJsonResponse(ret);
//         resp->setStatusCode(HttpStatusCode::k500InternalServerError);
//         callback(resp);
//     }
// }

void PersonsController::createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Person &&pPerson) const {
    LOG_DEBUG << "createOne";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        auto person = mp.insertFuture(pPerson).get();

        Json::Value ret{};
        ret = person.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k201Created);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int personId, Person &&pPerson) const {
    LOG_DEBUG << "updateOne personId: " << personId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        Person person;
        try {
            person = mp.findFutureByPrimaryKey(personId).get();
        } catch (const DrogonDbException & e) {
            Json::Value ret{};
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        if (pPerson.getJobId() != nullptr) {
          person.setJobId(pPerson.getValueOfJobId());
        }
        if (pPerson.getManagerId() != nullptr) {
          person.setManagerId(pPerson.getValueOfManagerId());
        }
        if (pPerson.getDepartmentId() != nullptr) {
          person.setDepartmentId(pPerson.getValueOfDepartmentId());
        }
        if (pPerson.getFirstName() != nullptr) {
          person.setFirstName(pPerson.getValueOfFirstName());
        }
        if (pPerson.getLastName() != nullptr) {
          person.setLastName(pPerson.getValueOfLastName());
        }

        mp.updateFuture(person).get();

        Json::Value ret{};
        ret = person.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int personId) const {
    LOG_DEBUG << "deleteOne personId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId)).get();

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::getDirectReports(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int personId) const {
    LOG_DEBUG << "getDirectReports personId: "<< personId;

    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    Person department;
    try {
        department = mp.findFutureByPrimaryKey(personId).get();
    } catch (const DrogonDbException & e) {
        Json::Value ret{};
        ret["error"] = "resource not found";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k404NotFound);
        callback(resp);
    }

    department.getPersons(dbClientPtr,
      [callbackPtr](const std::vector<Person> persons) {
          if (persons.empty()) {
             Json::Value ret{};
             ret["error"] = "resource not found";
             auto resp = HttpResponse::newHttpJsonResponse(ret);
             resp->setStatusCode(HttpStatusCode::k404NotFound);
             (*callbackPtr)(resp);
          } else {
             Json::Value ret{};
             for (auto p : persons) {
                 ret.append(p.toJson());
             }
             auto resp = HttpResponse::newHttpJsonResponse(ret);
             resp->setStatusCode(HttpStatusCode::k200OK);
             (*callbackPtr)(resp);
          }
      },
      [callbackPtr](const DrogonDbException &e) {
          LOG_ERROR << e.base().what();
          Json::Value ret{};
          ret["error"] = "database error";
          auto resp = HttpResponse::newHttpJsonResponse(ret);
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          (*callbackPtr)(resp);
      });
}

PersonsController::PersonDetails::PersonDetails(const PersonInfo &personInfo) {
    id = personInfo.getValueOfId();
    first_name = personInfo.getValueOfFirstName();
    last_name = personInfo.getValueOfLastName();
    hire_date = personInfo.getValueOfHireDate();
    Json::Value managerJson{};
    managerJson["id"] = personInfo.getValueOfManagerId();
    managerJson["full_name"] = personInfo.getValueOfManagerFullName();
    this->manager = managerJson;
    Json::Value departmentJson{};
    departmentJson["id"] = personInfo.getValueOfDepartmentId();
    departmentJson["name"] = personInfo.getValueOfDepartmentName();
    this->department = departmentJson;
    Json::Value jobJson{};
    jobJson["id"] = personInfo.getValueOfJobId();
    jobJson["title"] = personInfo.getValueOfJobTitle();
    this->job = jobJson;
}

auto PersonsController::PersonDetails::toJson() -> Json::Value {
    Json::Value ret{};
    ret["id"] = id;
    ret["first_name"] = first_name;
    ret["last_name"] = last_name;
    ret["hire_date"] = hire_date.toDbStringLocal();
    ret["manager"] = manager;
    ret["department"] = department;
    ret["job"] = job;
    return ret;
}
