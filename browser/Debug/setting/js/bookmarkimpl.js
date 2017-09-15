
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

function BookmarkQuery(a)
{
	this.m_type = a;

}

BookmarkQuery.prototype.Query = function (qtype) 
								{

									return  result = call("bookmark_query");							
								}


BookmarkQuery.prototype.Delete = function (tuple) 
								{
									return  result = call("bookmark_delete", tuple);
								}

BookmarkQuery.prototype.Clear = function () 
								{
									return  result = call("bookmark_clear");
								}

BookmarkQuery.prototype.Search = function (keyword) 
								{
									alert(keyword);
									return  result = call("bookmark_search", keyword);
								}

  	

