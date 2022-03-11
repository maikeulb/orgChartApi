#pragma once

#include <drogon/HttpController.h>
#include <string>
#include "../models/Person.h"
#include "../models/PersonInfo.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class PersonsController : public drogon::HttpController<PersonsController> {
 public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(PersonsController::get, "/persons", Get);
      ADD_METHOD_TO(PersonsController::getOne, "/persons/{1}", Get);
      ADD_METHOD_TO(PersonsController::createOne, "/persons", Post);
      ADD_METHOD_TO(PersonsController::updateOne, "/persons/{1}", Put);
      ADD_METHOD_TO(PersonsController::deleteOne, "/persons/{1}", Delete);
      ADD_METHOD_TO(PersonsController::getDirectReports, "/persons/{1}/reports", Get);
    METHOD_LIST_END

    void get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void getOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback, int pPersonId) const;
    void createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Person &&pPerson) const;
    void updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pPersonId, Person &&pPerson) const;
    void deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pPersonId) const;
    void getDirectReports(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pPersonId) const;

 private:
    struct PersonDetails {
        int id;
        std::string first_name;
        std::string last_name;
        trantor::Date hire_date;
        Json::Value manager;
        Json::Value department;
        Json::Value job;
        PersonDetails() {}
        explicit PersonDetails(const PersonInfo &personInfo);
        Json::Value toJson();
    };
};
