#include "PersonsController.h"
#include "../models/Person.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Person fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
        auto jsonVal = (*jsonPtr);
        auto person = Person(jsonVal);
        return person;
    }
}

// add offset and limit as optional parameters
void PersonsController::getAllPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const
{
    LOG_DEBUG << "index" << " offset, " << offset << " limit, " << limit;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    auto persons = mp.orderBy(Person::Cols::_id).limit(limit).offset(offset).findAll();

    Json::Value ret;
    for (auto p : persons) {
        ret.append(p.toJson());
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::getPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "getPerson personId: "<< personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    auto person = mp.findOne(Criteria(Person::Cols::_id, CompareOperator::EQ, personId)); // person

    // void getDepartment(const DbClientPtr &dbClientPtr, 
                  // const std::function<void(Products)> &rcb,
                  // const ExceptionCallback &ecb) const;

    // void getJob(const DbClientPtr &dbClientPtr, 
                  // const std::function<void(Products)> &rcb,
                  // const ExceptionCallback &ecb) const;

    Json::Value ret;
    ret = person.toJson(); 

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::newPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Person &&pNewPerson) const
{
    LOG_DEBUG << "newPerson";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    mp.insert(pNewPerson);

    Json::Value ret;
    ret["result"] = pNewPerson.toJson();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::updatePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Person &&pPersonDetails) const
{
    LOG_DEBUG << "updatePerson personId: " << personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    auto person = mp.findOne(Criteria(Person::Cols::_id, CompareOperator::EQ, personId));

    if (pPersonDetails.getJobId() != nullptr) {
      person.setJobId(pPersonDetails.getValueOfJobId());
    }
    if (pPersonDetails.getManagerId() != nullptr) {
      person.setManagerId(pPersonDetails.getValueOfManagerId());
    }
    if (pPersonDetails.getDepartmentId() != nullptr) {
      person.setDepartmentId(pPersonDetails.getValueOfDepartmentId());
    }
    if (pPersonDetails.getFirstName() != nullptr) {
      person.setFirstName(pPersonDetails.getValueOfFirstName());
    }
    if (pPersonDetails.getLastName() != nullptr) {
      person.setLastName(pPersonDetails.getValueOfLastName());
    }
    
    mp.update(person);

    Json::Value ret;
    ret["result"] = person.toJson();

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::deletePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "deletePerson, personId: ";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    mp.deleteBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId));

    Json::Value ret;
    ret["result"] = "OK";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::getDirectReports(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "getDirectReports personId: "<< personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    auto dep = mp.findOne(Criteria(Person::Cols::_id, CompareOperator::EQ, personId));

    dep.getPersons(dbClientPtr, 
      [callback](const std::vector<Person> persons) {
          Json::Value ret;
          for (auto p : persons) {
              ret.append(p.toJson());
          }
          auto resp=HttpResponse::newHttpJsonResponse(ret);
          callback(resp);
      },
      [](const DrogonDbException &e) {
          LOG_ERROR << "error:" << e.base().what();
      }
    );
}
