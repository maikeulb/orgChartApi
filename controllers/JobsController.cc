#include "JobsController.h"
#include "../models/Job.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Job fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
				auto jsonVal = (*jsonPtr);
				auto job = Job(jsonVal);
				return job;
    }
}

void JobsController::index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
		LOG_DEBUG << "index";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    auto jobs = mp.orderBy(Job::Cols::_id).limit(25).offset(0).findAll();

    Json::Value ret;
    for (auto j : jobs) {
        ret.append(j.toJson());
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::getJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
		LOG_DEBUG << "getJob jobId: "<< jobId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
		auto emp = mp.findBy(Criteria(Job::Cols::_id, CompareOperator::EQ, jobId));

    Json::Value ret;
		ret = emp[0].toJson(); 

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::newJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Job &&pNewJob) const
{
		LOG_DEBUG << "newJob";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    mp.insert(pNewJob);

    Json::Value ret;
    ret["result"] = pNewJob.toJson();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::updateJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Job &&pJobDetails) const
{
		LOG_DEBUG << "updateJob personId: " << personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
		auto emp = mp.findBy(Criteria(Job::Cols::_id, CompareOperator::EQ, personId))[0];

		if (pJobDetails.getTitle() != nullptr) {
				emp.setTitle(pJobDetails.getValueOfTitle());
		}
		
    mp.update(emp);

    Json::Value ret;
    ret["result"] = emp.toJson();

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::deleteJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
		LOG_DEBUG << "deleteJob, personId: ";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
		mp.deleteBy(Criteria(Job::Cols::_id, CompareOperator::EQ, personId));

    Json::Value ret;
    ret["result"] = "OK";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
