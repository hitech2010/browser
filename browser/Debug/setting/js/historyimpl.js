
function test()
{
	return  external.test("123", 5,true,7.3,"890");
}

function asyncall(a,b,c,d,e)
{
	return  external.jnotify_asyn(a,b,c,d,e)
}


function call(a,b,c,d,e)
{
	return external.jnotify(a,b,c,d,e)
}


var items = new Array();
var search_items = new Array();
var page = 0;

function HistoryQuery(a)
{
	this.m_type = a;

}



HistoryQuery.prototype.Query = function (qtype) 
								{
									if(qtype == "today")
									{
										var  result = call("history_query_today");	
										return result;
									}

									if(qtype == "yesterday")
									{
										return  result = call("history_query_yesterday");	
									}

									if(qtype == "thisweek")
									{
										return  result = call("history_query_thisweek");	
									}

									if(qtype == "thismonth")
									{
										return  result = call("history_query_thismonth");	
									}
									
								}


HistoryQuery.prototype.Delete = function (tuple) 
								{
									return  result = call("history_delete", tuple);
								}

HistoryQuery.prototype.Clear = function () 
								{
									return  result = call("history_clear");
								}

HistoryQuery.prototype.Search = function (keyword) 
								{
									
									return  result = call("history_search", keyword);
								}

  	

