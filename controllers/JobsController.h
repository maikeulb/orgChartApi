#pragma once

#include <drogon/HttpController.h>
#include "../models/Job.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class JobsController : public drogon::HttpController<JobsController> {
 public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(JobsController::get, "/jobs", Get, "LoginFilter");
      ADD_METHOD_TO(JobsController::getOne, "/jobs/{1}", Get, "LoginFilter");
      ADD_METHOD_TO(JobsController::createOne, "/jobs", Post, "LoginFilter");
      ADD_METHOD_TO(JobsController::updateOne, "/jobs/{1}", Put, "LoginFilter");
      ADD_METHOD_TO(JobsController::deleteOne, "/jobs/{1}", Delete, "LoginFilter");
      ADD_METHOD_TO(JobsController::getJobPersons, "/jobs/{1}/persons", Get, "LoginFilter");
    METHOD_LIST_END

    void get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void getOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback, int pJobId) const;
    void createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Job &&pJob) const;
    void updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pJobId, Job &&pJob) const;
    void deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pJobId) const;
    void getJobPersons(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int jobId) const;
};
