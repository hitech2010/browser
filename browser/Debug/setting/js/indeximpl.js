
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


var page = 0;

function IndexPage(a)
{
	this.m_type = a;

}


IndexPage.prototype.Query = function () 
								{
									return  result = call("index_query");
								}

IndexPage.prototype.Delete = function(index)
								{
									return  result = call("index_delete_history", index);
								}

  	

