#pragma once

#include <drogon/HttpController.h>
#include "../models/Person.h"
#include "../models/Department.h"
#include "../models/Job.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class PersonsController:public drogon::HttpController<PersonsController>
{
  public:
    METHOD_LIST_BEGIN
			ADD_METHOD_TO(PersonsController::newPerson, "/persons", Post); 
		  ADD_METHOD_TO(PersonsController::getAllPersons,"/persons?offset={1}&limit={2}", Get); 
		  ADD_METHOD_TO(PersonsController::getPerson,"/persons/{1}", Get);
      ADD_METHOD_TO(PersonsController::updatePerson,"/persons/{1}", Put);
      ADD_METHOD_TO(PersonsController::deletePerson,"/persons/{1}", Delete);
      ADD_METHOD_TO(PersonsController::getDirectReports,"/persons/{1}/reports", Get); 
    METHOD_LIST_END

    void getAllPersons(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const;
    void getPerson(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pPersonId) const;
    void newPerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Person &&pPerson) const;
		void updatePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pPersonId, Person &&pPerson) const;
    void deletePerson(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pPersonId) const;
    void getDirectReports(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pPersonId) const;

  private:
    struct PersonDetails {
        int id;
        std::string first_name;
        std::string last_name;
        trantor::Date hire_date;
        Json::Value manager;
        Json::Value department;
        Json::Value job;
        PersonDetails() {};
        explicit PersonDetails(const Person &person, const Person &manager, const Department &department, const Job &job);
        auto toJson() -> Json::Value;
    };
};
