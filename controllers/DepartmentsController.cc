#include "DepartmentsController.h"
#include "../models/Department.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Department fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
				auto jsonVal = (*jsonPtr);
				auto department = Department(jsonVal);
				return department;
    }
}

void DepartmentsController::index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
		LOG_DEBUG << "index";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto departments = mp.orderBy(Department::Cols::_id).limit(25).offset(0).findAll();

    Json::Value ret;
    for (auto j : departments) {
        ret.append(j.toJson());
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::getDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
		LOG_DEBUG << "getDepartment departmentId: "<< departmentId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
		auto emp = mp.findBy(Criteria(Department::Cols::_id, CompareOperator::EQ, departmentId));

    Json::Value ret;
		ret = emp[0].toJson(); 

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::newDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Department &&pNewDepartment) const
{
		LOG_DEBUG << "newDepartment";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.insert(pNewDepartment);

    Json::Value ret;
    ret["result"] = pNewDepartment.toJson();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::updateDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Department &&pDepartmentDetails) const
{
		LOG_DEBUG << "updateDepartment personId: " << personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
		auto emp = mp.findBy(Criteria(Department::Cols::_id, CompareOperator::EQ, personId))[0];

		if (pDepartmentDetails.getName() != nullptr) {
				emp.setName(pDepartmentDetails.getValueOfName());
		}
		
    mp.update(emp);

    Json::Value ret;
    ret["result"] = emp.toJson();

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::deleteDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
		LOG_DEBUG << "deleteDepartment, personId: ";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
		mp.deleteBy(Criteria(Department::Cols::_id, CompareOperator::EQ, personId));

    Json::Value ret;
    ret["result"] = "OK";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
