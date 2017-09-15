
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

function BrowserSetting(a)
{
	this.m_type = a;

}

BrowserSetting.prototype.Query = function (qtype) 
								{
									return result = call("setting_query");
								}


BrowserSetting.prototype.Update = function (jstring) 
								{
									return  result = call("setting_update", jstring);
								}

  	

