#pragma once

#include <drogon/HttpController.h>
#include "../models/Department.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class DepartmentsController:public drogon::HttpController<DepartmentsController>
{
  public:
    METHOD_LIST_BEGIN
			ADD_METHOD_TO(DepartmentsController::newDepartment, "/departments", Post); 
		  ADD_METHOD_TO(DepartmentsController::getAllDepartments,"/departments?offset={1}&limit={2}", Get); 
		  ADD_METHOD_TO(DepartmentsController::getDepartment,"/departments/{1}", Get); 
      ADD_METHOD_TO(DepartmentsController::updateDepartment,"/departments/{1}", Put);
      ADD_METHOD_TO(DepartmentsController::deleteDepartment,"/departments/{1}", Delete); 
      ADD_METHOD_TO(DepartmentsController::getDepartmentPersons,"/departments/{1}/persons", Get); 
    METHOD_LIST_END

    void getAllDepartments(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const;
    void getDepartment(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
    void newDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Department &&pNewDepartment) const;
		void updateDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId, Department &&pDepartmentDetails) const;
    void deleteDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
    void getDepartmentPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const;
};
