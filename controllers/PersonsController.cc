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

void PersonsController::index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
		LOG_DEBUG << "index";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
    auto persons = mp.orderBy(Person::Cols::_id).limit(25).offset(0).findAll();

    Json::Value ret;
    for (auto j : persons) {
        ret.append(j.toJson());
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void PersonsController::getPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
		LOG_DEBUG << "getPerson personId: "<< personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Person> mp(dbClientPtr);
		auto emp = mp.findBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId));

    Json::Value ret;
		ret = emp[0].toJson(); 

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
		auto emp = mp.findBy(Criteria(Person::Cols::_id, CompareOperator::EQ, personId))[0];

		if (pPersonDetails.getJobId() != nullptr) {
				emp.setJobId(pPersonDetails.getValueOfJobId());
		}
		if (pPersonDetails.getManagerId() != nullptr) {
				emp.setManagerId(pPersonDetails.getValueOfManagerId());
		}
		if (pPersonDetails.getDepartmentId() != nullptr) {
				emp.setDepartmentId(pPersonDetails.getValueOfDepartmentId());
		}
		if (pPersonDetails.getFirstName() != nullptr) {
				emp.setFirstName(pPersonDetails.getValueOfFirstName());
		}
		if (pPersonDetails.getLastName() != nullptr) {
				emp.setLastName(pPersonDetails.getValueOfLastName());
		}
		
    mp.update(emp);

    Json::Value ret;
    ret["result"] = emp.toJson();

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
