#pragma once

#include <drogon/HttpController.h>
#include "../models/Department.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class DepartmentsController:public drogon::HttpController<DepartmentsController>
{
  public:
    METHOD_LIST_BEGIN
			ADD_METHOD_TO(DepartmentsController::newDepartment, "/departments/", Post); // path is /departments/
		  ADD_METHOD_TO(DepartmentsController::index,"/departments/", Get); //path is /departments/list
		  ADD_METHOD_TO(DepartmentsController::getDepartment,"/departments/{1}", Get); //path is /departments/{arg1}
      ADD_METHOD_TO(DepartmentsController::updateDepartment,"/departments/{1}", Put); //path is /departments/list
      ADD_METHOD_TO(DepartmentsController::deleteDepartment,"/departments/{1}", Delete); //path is /departments/list
    METHOD_LIST_END

    void index(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const;
    void getDepartment(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
    void newDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Department &&pNewDepartment) const;
		void updateDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId, Department &&pDepartmentDetails) const;
    void deleteDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
};
