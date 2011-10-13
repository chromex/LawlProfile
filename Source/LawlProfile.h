#ifndef LAWLPROFILE_H
#define LAWLPROFILE_H

#include "LawlProfile_Settings.h"
#include "LawlProfile_Time.h"

#include <string>
#include <map>

BEGIN_LAWLPROFILE

class LawlProfiler
{
	struct PNode;
	typedef std::map<std::string, PNode*> PNodeMap;

	struct PNode
	{
		LP_Time::TimeVal     _totalTime;
		int         _callCount;
		PNodeMap    _children;
		std::string _name;
		PNode *     _parent;
		LP_Time::TimeVal     _startTime;

		PNode() : _name("ROOT"), _callCount(1), _totalTime(0), _parent(NULL) {}
		PNode(const std::string& name, PNode* parent) 
			: _name(name), _callCount(0), _totalTime(0), _parent(parent) {}

		~PNode()
		{
			for(PNodeMap::iterator iter = _children.begin();
				iter != _children.end();
				++iter)
			{
				delete iter->second;
			}
		}
	};

public:
	/* 'structors */
	~LawlProfiler();

	/* Singleton */
	static LawlProfiler& inst();

	/* Clear the tree */
	void Clear(bool reset = true);

	/* Output */
	std::string DumpLog(const std::string& filename) const;

	/* Modifiers */
	void Enter(const std::string& name);
	void Exit();

	/* Simple object for auto timer release */
	class AutoTimer
	{
	public: ~AutoTimer() {inst().Exit();}
	};

private:
	/* Output helper */
	void TreeTraverse(int depth, const PNode *current, 
		std::stringstream& ss) const;
	LP_Time::TimeVal SumChildren(const PNode* node) const;

	/* Call tree */
	PNode *_root;
	PNode *_current;

	/* Render meta */
	mutable int _line;

	/* Protect the singleton */
	LawlProfiler() {_root = new PNode; _current = _root;}
	LawlProfiler(const LawlProfiler& other) {}
	LawlProfiler& operator=(const LawlProfiler& other) {}
};

#ifdef LAWLPROFILE_PROF_ENABLED
#define TIME_BEGIN(S) LawlProfiler::inst().Enter(#S);
#define TIME_END() LawlProfiler::inst().Exit();
#define TIME_AUTO(S) LawlProfiler::AutoTimer __at; TIME_BEGIN(S)
#elif
#define TIME_BEGIN(S)
#define TIME_END()
#define TIME_AUTO(S)
#endif

#endif

END_LAWLPROFILE