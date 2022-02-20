#include "JobsController.h"
#include "../models/Job.h"
#include "../models/Person.h"

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
    auto job = mp.findOne(Criteria(Job::Cols::_id, CompareOperator::EQ, jobId));

    Json::Value ret;
    ret = job.toJson(); 

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

void JobsController::updateJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId, Job &&pJobDetails) const
{
    LOG_DEBUG << "updateJob jobId: " << jobId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    auto job = mp.findOne(Criteria(Job::Cols::_id, CompareOperator::EQ, jobId));

    if (pJobDetails.getTitle() != nullptr) {
        job.setTitle(pJobDetails.getValueOfTitle());
    }
    
    mp.update(job);

    Json::Value ret;
    ret["result"] = job.toJson();

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::deleteJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "deleteJob, jobId: ";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    mp.deleteBy(Criteria(Job::Cols::_id, CompareOperator::EQ, jobId));

    Json::Value ret;
    ret["result"] = "OK";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void JobsController::getJobPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "getJobPersons jobId: "<< jobId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    auto dep = mp.findOne(Criteria(Job::Cols::_id, CompareOperator::EQ, jobId));

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
            LOG_DEBUG << "error:" << e.base().what();
        }
    );
}
