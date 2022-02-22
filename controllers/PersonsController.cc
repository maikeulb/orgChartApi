#include "PersonsController.h"
#include "../models/Person.h"
#include "../models/Department.h"
#include "../models/Job.h"

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
}

void PersonsController::get(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "get";
    auto offset = req->getOptionalParameter<int>("offset").value_or(0);
    auto limit = req->getOptionalParameter<int>("limit").value_or(25);
    auto sortField = req->getOptionalParameter<std::string>("sort_field").value_or("id");
    auto sortOrder = req->getOptionalParameter<std::string>("sort_order").value_or("asc");
    auto sortOrderEnum = sortOrder == "asc" ? SortOrder::ASC : SortOrder::DESC;

    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> pmp(dbClientPtr);
        Mapper<Department> dmp(dbClientPtr);
        Mapper<Job> jmp(dbClientPtr);

        auto persons = pmp.orderBy(sortField, sortOrderEnum).offset(offset).limit(limit).findFutureAll().get();

        Json::Value ret;
        for (auto p : persons) {
            auto manager = pmp.findFutureByPrimaryKey(p.getValueOfManagerId());
            auto department = dmp.findFutureByPrimaryKey(p.getValueOfDepartmentId()); 
            auto job = jmp.findFutureByPrimaryKey(p.getValueOfJobId()); 
            PersonDetails personDetails = PersonDetails(p, manager.get(), department.get(), job.get());
            ret.append(personDetails.toJson());
        }
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::getOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "getOne personId: "<< personId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> pmp(dbClientPtr);
        Mapper<Department> dmp(dbClientPtr);
        Mapper<Job> jmp(dbClientPtr);

        Person person;
        try {
            person = pmp.findFutureByPrimaryKey(personId).get(); 
        } catch (const DrogonDbException & e) {
            Json::Value ret;
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        auto manager = pmp.findFutureByPrimaryKey(person.getValueOfManagerId());
        auto department = dmp.findFutureByPrimaryKey(person.getValueOfDepartmentId()); 
        auto job = jmp.findFutureByPrimaryKey(person.getValueOfJobId()); 
        PersonDetails personDetails = PersonDetails(person, manager.get(), department.get(), job.get());

        Json::Value ret = personDetails.toJson(); 
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::createOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Person &&pPerson) const
{
    LOG_DEBUG << "createOne";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        auto person = mp.insertFuture(pPerson).get();
        
        Json::Value ret = person.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k201Created);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::updateOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Person &&pPerson) const
{
    LOG_DEBUG << "updateOne personId: " << personId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        Person person;
        try {
            person = mp.findFutureByPrimaryKey(personId).get(); 
        } catch (const DrogonDbException & e) {
            Json::Value ret;
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

        Json::Value ret = person.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::deleteOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "deleteOne personId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId)).get();

        auto resp=HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::getDirectReports(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "getDirectReports personId: "<< personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    Person department;
    try {
        department = mp.findFutureByPrimaryKey(personId).get();
    } catch (const DrogonDbException & e) {
        Json::Value ret;
        ret["error"] = "resource not found";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k404NotFound);
        callback(resp);
    }

    department.getPersons(dbClientPtr, 
      [callback](const std::vector<Person> persons) {
          if (persons.empty()) {
             Json::Value ret;
             ret["error"] = "resource not found";
             auto resp = HttpResponse::newHttpJsonResponse(ret);
             resp->setStatusCode(HttpStatusCode::k404NotFound);
             callback(resp);
          } else {
             Json::Value ret;
             for (auto p : persons) {
                 ret.append(p.toJson());
             }
             auto resp = HttpResponse::newHttpJsonResponse(ret);
             resp->setStatusCode(HttpStatusCode::k200OK);
             callback(resp);
          }
      },
      [callback](const DrogonDbException &e) {
          LOG_ERROR << e.base().what();
          Json::Value ret;
          ret["error"] = "database error";
          auto resp = HttpResponse::newHttpJsonResponse(ret);
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          callback(resp);
      }
    );
}

PersonsController::PersonDetails::PersonDetails(const Person &person, const Person &manager, const Department &department, const Job &job) {
    id = person.getValueOfId();
    first_name = person.getValueOfFirstName();
    last_name = person.getValueOfLastName();
    hire_date = person.getValueOfHireDate();
    this->manager = manager.toJson();
    this->department = department.toJson();
    this->job = job.toJson();
}

auto PersonsController::PersonDetails::toJson() -> Json::Value {
    Json::Value ret;
    ret["id"] = id;
    ret["first_name"] = first_name;
    ret["last_name"] = last_name;
    ret["hire_date"] = hire_date.toDbStringLocal();
    ret["manager"] = manager;
    ret["department"] = department;
    ret["job"] = job;
    return ret;
}
