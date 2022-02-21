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

void PersonsController::getAllPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const
{
    LOG_DEBUG << "getAllPersons" << " offset: " << offset << " limit: " << limit;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> personMp(dbClientPtr);
        Mapper<Department> departmentMp(dbClientPtr);
        Mapper<Job> jobMp(dbClientPtr);

        auto persons = personMp.orderBy(Person::Cols::_id).offset(offset).limit(limit).findFutureAll().get();
        Json::Value ret;
        for (auto p : persons) {
            auto manager = personMp.findFutureByPrimaryKey(p.getValueOfManagerId());
            auto department = departmentMp.findFutureByPrimaryKey(p.getValueOfDepartmentId()); 
            auto job = jobMp.findFutureByPrimaryKey(p.getValueOfJobId()); 
            PersonDetails personDetails = PersonDetails(p, manager.get(), department.get(), job.get());
            ret.append(personDetails.toJson());
        }
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::getPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "getPerson personId: "<< personId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> personMp(dbClientPtr);
        Mapper<Department> departmentMp(dbClientPtr);
        Mapper<Job> jobMp(dbClientPtr);

        Person person;
        try {
            person = personMp.findFutureByPrimaryKey(personId).get(); 
        } catch (const DrogonDbException & e) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        auto manager = personMp.findFutureByPrimaryKey(person.getValueOfManagerId());
        auto department = departmentMp.findFutureByPrimaryKey(person.getValueOfDepartmentId()); 
        auto job = jobMp.findFutureByPrimaryKey(person.getValueOfJobId()); 
        PersonDetails personDetails = PersonDetails(person, manager.get(), department.get(), job.get());

        Json::Value ret = personDetails.toJson(); 
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::newPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Person &&pPerson) const
{
    LOG_DEBUG << "newPerson";
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
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::updatePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Person &&pPerson) const
{
    LOG_DEBUG << "updatePerson personId: " << personId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        Person person;
        try {
            person = mp.findFutureByPrimaryKey(personId).get(); 
        } catch (const DrogonDbException & e) {
            auto resp = HttpResponse::newHttpResponse();
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
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void PersonsController::deletePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "deletePerson personId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Person> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId)).get();

        auto resp=HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
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
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k404NotFound);
        callback(resp);
    }

    department.getPersons(dbClientPtr, 
      [callback](const std::vector<Person> persons) {
          if (persons.empty()) {
             auto resp = HttpResponse::newHttpResponse();
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
          auto resp = HttpResponse::newHttpResponse();
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
