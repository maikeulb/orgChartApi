#pragma once

#include <drogon/HttpController.h>
#include "../models/Job.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class JobsController:public drogon::HttpController<JobsController>
{
  public:
    METHOD_LIST_BEGIN
			ADD_METHOD_TO(JobsController::newJob, "/jobs", Post); 
		  ADD_METHOD_TO(JobsController::getAllJobs,"/jobs?offset={1}&limit={2}", Get); 
		  ADD_METHOD_TO(JobsController::getJob,"/jobs/{1}", Get);
      ADD_METHOD_TO(JobsController::updateJob,"/jobs/{1}", Put);
      ADD_METHOD_TO(JobsController::deleteJob,"/jobs/{1}", Delete); 
      ADD_METHOD_TO(JobsController::getJobPersons,"/jobs/{1}/persons", Get); 
    METHOD_LIST_END

    void newJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Job &&pJob) const;
    void getAllJobs(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const;
    void getJob(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pJobId) const;
		void updateJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pJobId, Job &&pJob) const;
    void deleteJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pJobId) const;
    void getJobPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const;
};
