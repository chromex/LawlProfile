#include "LawlProfile.h"

#include <sstream>
#include <iomanip>
#include <cassert>

BEGIN_LAWLPROFILE

LawlProfiler::~LawlProfiler()
{
	clear(false);
}

LawlProfiler& LawlProfiler::inst()
{
	static LawlProfiler p;
	return p;
}

void LawlProfiler::clear(bool reset)
{
	if(_root)
		delete _root;
	if(reset)
		_root = new PNode;
}

std::string LawlProfiler::dumpLog( const std::string& filename ) const
{
	const char header[] = 
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\
		<html>\
			<head>\
				<title>Arrogance Profiler</title>\
				<style type\"text/css\">\
					@import url(table.css);\
				</style>\
				<script src=\"gui.js\" type=\"text/javascript\"></script>\
				<style type=\"text/css\">\
				body {font-size: 0.9em;}\
				table {border-collapse:collapse;}\
				.odd {background-color:#ccc;}\
				.even {background-color:#eee;}\
				.cells {text-align:right;border-width: 0px 0px 1px 0px;border-color:#999;border-style:dashed;}\
				\
				</style>\
			</head>\
			<body>\
			<table id=\"tbl\">\
				<tbody>\
				<tr>\
					<th class=\"TABLE_NONE\">Scope</th>\
					<th class=\"TABLE_NONE\" onclick=\"orderColumn('tbl', 1, false)\">Call #</th>\
					<th class=\"TABLE_NONE\" onclick=\"orderColumn('tbl', 2, false)\">Total MS</th>\
					<th class=\"TABLE_NONE\" onclick=\"orderColumn('tbl', 3, false)\">Avg MS</th>\
					<th class=\"TABLE_NONE\" onclick=\"orderColumn('tbl', 4, false)\">Inclusive MS</th>\
					<th class=\"TABLE_NONE\" onclick=\"orderColumn('tbl', 5, false)\">Exlusive MS</th>\
				</tr>\
		";

	std::stringstream ss;
	ss << std::setprecision(2) << std::fixed;
	ss << header << std::endl;
	_line = 1;
	treeTraverse(0, _root, ss);
	ss << "</tbody></table></body></html>" << std::endl;

	return ss.str();
}

void LawlProfiler::enter( const std::string& name )
{
	// Check current hash map for existing child
	if(_current->_children.find(name) == _current->_children.end())
	{
		// Create if it doesn't exist
		_current->_children[name] = new PNode(name, _current);
	}

	// Set current
	_current = _current->_children[name];

	// Inc counter
	++_current->_callCount;

	// Capture start time
	_current->_startTime = GetSysTime();
}

void LawlProfiler::exit()
{
	// Capture end time
	TimeVal diff = GetSysTime() - _current->_startTime;

	// Inc total time
	_current->_totalTime += diff;

	// Set current to _parent
	_current = _current->_parent;
	assert(_current != NULL);
}

void LawlProfiler::treeTraverse( int depth, const PNode *current, 
							std::stringstream& ss ) const
{
	double t = ConvertSysTime(current->_totalTime) * 1000.0;
	double childTime = ConvertSysTime(sumChildren(current)) * 1000.0;
	double inclusive = t - childTime;

	if(_line % 2 == 0)
		ss << "<tr class=\"odd\">";
	else
		ss << "<tr class=\"even\">";
	int d = depth;
	if(d == 0) d = 1;
	ss << "<td class=\"cells\" style=\"text-align:left;\">"; //padding-left:" << (d-1)*20 << "px;
	for(int i = 0; i < d-1; ++i)
	{
		ss << "&nbsp;&nbsp;&nbsp;";
	}
	if(depth >= 1)
		ss << "<img src=\"subtree.png\">";
	ss << current->_name << "</td>";
	ss << "<td class=\"cells\">" << current->_callCount << "</td>";
	ss << "<td class=\"cells\">" << t << "</td>";
	ss << "<td class=\"cells\">" << t / current->_callCount << "</td>";
	ss << "<td class=\"cells\">" << inclusive << "</td>";
	ss << "<td class=\"cells\">" << childTime << "</td>";
	ss << "</tr>" << std::endl;

	++_line;

	for(PNodeMap::const_iterator iter = current->_children.begin();
		iter != current->_children.end();
		++iter)
	{
		treeTraverse(depth+1, iter->second, ss);
	}
}

TimeVal LawlProfiler::sumChildren( const PNode* node ) const
{
	TimeVal t = 0;

	for(PNodeMap::const_iterator iter = node->_children.begin();
		iter != node->_children.end();
		++iter)
	{
		t += iter->second->_totalTime;
	}

	return t;
}

END_LAWLPROFILE