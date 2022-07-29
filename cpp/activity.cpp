#include "binaryninjacore/activity.h"
#include "binaryninjacore/basicblock.h"
#include "json/json.h"

#include "binaryninja/getobject.hpp"
#include "binaryninja/architecture.hpp"
#include "binaryninja/confidence.hpp"
#include "binaryninja/activity.hpp"
#include <string>
#include <typeinfo>

using namespace BinaryNinja;
using namespace std;

AnalysisContext::AnalysisContext(BNAnalysisContext* analysisContext) :
    m_reader(Json::CharReaderBuilder().newCharReader())
{
	// LogError("API-Side AnalysisContext Constructed!");
	m_object = analysisContext;
	m_builder["indentation"] = "";
}


AnalysisContext::~AnalysisContext()
{
	// LogError("API-Side AnalysisContext Destructed!");
}


Ref<Function> AnalysisContext::GetFunction()
{
	BNFunction* func = BNAnalysisContextGetFunction(m_object);
	if (!func)
		return nullptr;
	return CreateNewFunction(func);
}


Ref<LowLevelILFunction> AnalysisContext::GetLowLevelILFunction()
{
	BNLowLevelILFunction* func = BNAnalysisContextGetLowLevelILFunction(m_object);
	if (!func)
		return nullptr;
	return CreateNewLowLevelILFunction(func);
}


Ref<MediumLevelILFunction> AnalysisContext::GetMediumLevelILFunction()
{
	BNMediumLevelILFunction* func = BNAnalysisContextGetMediumLevelILFunction(m_object);
	if (!func)
		return nullptr;
	return CreateNewMediumLevelILFunction(func);
}


Ref<HighLevelILFunction> AnalysisContext::GetHighLevelILFunction()
{
	BNHighLevelILFunction* func = BNAnalysisContextGetHighLevelILFunction(m_object);
	if (!func)
		return nullptr;
	return CreateNewHighLevelILFunction(func);
}


void AnalysisContext::SetBasicBlockList(vector<Ref<BasicBlock>> basicBlocks)
{
	BNBasicBlock** blocks = new BNBasicBlock*[basicBlocks.size()];
	size_t i = 0;
	for (auto& j : basicBlocks)
		blocks[i++] = BinaryNinja::GetObject(j);

	BNSetBasicBlockList(m_object, blocks, basicBlocks.size());
	delete[] blocks;
}


void AnalysisContext::SetLiftedILFunction(Ref<LowLevelILFunction> liftedIL)
{
	BNSetLiftedILFunction(m_object, BinaryNinja::GetObject(liftedIL));
}


void AnalysisContext::SetLowLevelILFunction(Ref<LowLevelILFunction> lowLevelIL)
{
	BNSetLowLevelILFunction(m_object, BinaryNinja::GetObject(lowLevelIL));
}


void AnalysisContext::SetMediumLevelILFunction(Ref<MediumLevelILFunction> mediumLevelIL)
{
	BNSetMediumLevelILFunction(m_object, BinaryNinja::GetObject(mediumLevelIL));
}


void AnalysisContext::SetHighLevelILFunction(Ref<HighLevelILFunction> highLevelIL)
{
	BNSetHighLevelILFunction(m_object, BinaryNinja::GetObject(highLevelIL));
}


bool AnalysisContext::Inform(const string& request)
{
	return BNAnalysisContextInform(m_object, request.c_str());
}


Activity::Activity(const string& name, const std::function<void(Ref<AnalysisContext> analysisContext)>& action) :
    m_action(action)
{
	// LogError("API-Side Activity Constructed!");
	m_object = BNCreateActivity(name.c_str(), this, Run);
}


Activity::Activity(BNActivity* activity)
{
	m_object = BNNewActivityReference(activity);
}


Activity::~Activity()
{
	// LogError("API-Side Activity Destructed!");
}


void Activity::Run(void* ctxt, BNAnalysisContext* analysisContext)
{
	Activity* activity = (Activity*)ctxt;
	Ref<AnalysisContext> ac = new AnalysisContext(BNNewAnalysisContextReference(analysisContext));
	activity->m_action(ac);
}


string Activity::GetName() const
{
	char* name = BNActivityGetName(m_object);
	string result = name;
	BNFreeString(name);
	return result;
}
