



function loading_settings(jappconfig)
{



	//page general(常规设置)
	$(".general .radio-selected").removeClass("radio-selected");
	if(jappconfig.startup_page_policy == "0")
	{
		var self = $("#startup_page_policy_0");
		self.parent().addClass("radio-selected");
	}
	else if(jappconfig.startup_page_policy == "1")
	{
		
		var self = $("#startup_page_policy_1");
		self.parent().addClass("radio-selected");
	}
	else
	{
		var self = $("#startup_page_policy_2");
		self.parent().addClass("radio-selected");	
	}

	$(".general .check-selected").removeClass("check-selected");
	if(jappconfig.ui_show_homepage == "1")
	{
		var self = $("#main-page");
		self.parent().addClass("check-selected");
	}
	if(jappconfig.ui_show_bookmark == "1")
	{
		
		var self = $("#show-bookmarks");
		self.parent().addClass("check-selected");
	}
	if(jappconfig.ui_show_shortcut == "1")
	{
		var self = $("#screenshot");
		self.parent().addClass("check-selected");	
	}
	if(jappconfig.ui_show_restore_recent == "1")
	{
		var self = $("#Restore");
		self.parent().addClass("check-selected");	
	}

	//page label setting(书签设置)
	//
	$(".label-setting .radio-selected").removeClass("radio-selected");
	$(".label-setting .check-selected").removeClass("check-selected");
	if(jappconfig.tabset_qiantaitiaozhuan == "1")
	{
		$("#Reception").parent().addClass("radio-selected");
	}
	else
	{
		$("#Backstage").parent().addClass("radio-selected");
	}
	


	if(jappconfig.tabset_close_dblclick == "1")  $("#double-click-off").addClass("check-selected");	

	if(jappconfig.tabset_close_rightclick == "1")$("#right-click-off").parent().addClass("check-selected");	

	if(jappconfig.tabset_quit_whencloselast == "1")$("#close-browser").parent().addClass("check-selected");	

	if(jappconfig.tabset_newtab_whenclickbookmark == "1") $("#opennewpage").parent().addClass("radio-selected");
	else $("#cover-page").parent().addClass("radio-selected");

	if(jappconfig.tabset_newtab_navigateaddress == "1") $("#opennewpage2").parent().addClass("radio-selected");
	else $("#cover-page2").parent().addClass("radio-selected");

	if(jappconfig.tabset_newtab_position == "0") $("#current-right-side").parent().addClass("radio-selected");
	else $("#Rightmost").parent().addClass("radio-selected");

	if(jappconfig.tabset_activepos_whenclosetab == "0") $("#Activate-left").parent().addClass("radio-selected");
	else $("#Activate-right").parent().addClass("radio-selected");



	//下载页面
	//
	$(".download .radio-selected").removeClass("radio-selected");
	$(".download .check-selected").removeClass("check-selected");


	
	$("#downloadLocationPath").removeAttr("readonly").attr("value",jappconfig.dnload_location);
	$("#downloadLocationPath").attr("readonly","readonly");	

	

	if(jappconfig.dnload_ask_befroe_newtask == "1")  $("#download-before").parent().addClass("check-selected");

	if(jappconfig.dnload_info_whenover == "1")  $("#download-complate").parent().addClass("check-selected");


	//截图设置
	$(".picture-format .radio-selected").removeClass("radio-selected");
	$(".picture-format .check-selected").removeClass("check-selected");

	
	if(jappconfig.shortcut_ext == "png") $("#png-format").parent().addClass("radio-selected");
	else $("#jpg-format").parent().addClass("radio-selected");

	$("#downloadLocationPath2").removeAttr("readonly").attr("value",jappconfig.location);
	$("#downloadLocationPath2").attr("readonly","readonly");	


		//默认搜索引擎

	var jslength=0;

	var nodes = "";

	for(var js2 in jappconfig.search_engines){

		var li = "";
		var licls = "";
		if(jappconfig.search_engines[js2][3])
		{


			li = '<li class="searchHover defaultEngines">\
						<p class="searchName">' + jappconfig.search_engines[js2][0] + '</p>\
						<p class="searchKey">'  + jappconfig.search_engines[js2][1] + '</p>\
						<p class="searchWeb">'  + jappconfig.search_engines[js2][2] + '</p>\
						<span class="setDefault">设为默认</span>\
						<span class="cutWeb"></span>\
				  </li>';
		}
		else
		{
			li = '<li class="searchHover">\
						<p class="searchName">' + jappconfig.search_engines[js2][0] + '</p>\
						<p class="searchKey">'  + jappconfig.search_engines[js2][1] + '</p>\
						<p class="searchWeb">'  + jappconfig.search_engines[js2][2] + '</p>\
						<span class="setDefault">设为默认</span>\
						<span class="cutWeb"></span>\
				  </li>';
		}

		nodes += li;
	}

	$(".searchBox").html(nodes);


	
	
}

function IsURL (str_url) 
{ 
	var strRegex = '^((https|http|ftp|rtsp|mms)?://)'
	+ '?(([0-9a-z_!~*\'().&=+$%-]+: )?[0-9a-z_!~*\'().&=+$%-]+@)?' //ftp的user@ 
	+ '(([0-9]{1,3}.){3}[0-9]{1,3}' // IP形式的URL- 199.194.52.184 
	+ '|' // 允许IP和DOMAIN（域名） 
	+ '([0-9a-z_!~*\'()-]+.)*' // 域名- www. 
	+ '([0-9a-z][0-9a-z-]{0,61})?[0-9a-z].' // 二级域名 
	+ '[a-z]{2,6})' // first level domain- .com or .museum 
	+ '(:[0-9]{1,4})?' // 端口- :80 
	+ '((/?)|' // a slash isn't required if there is no file name 
	+ '(/[0-9a-z_!~*\'().;?:@&=+$,%#-]+)+/?)$'; 
	var re=new RegExp(strRegex); 
	//re.test() 
	if (re.test(str_url)) { 
	return (true); 
	} else { 
	return (false); 
	} 
}

function sync_settings(jconfig)
{
		//page general(常规设置)
		//
		//

	var jappconfig = jconfig.appconfig;
	
	if(     $("#startup_page_policy_0").parent().is(".radio-selected"))
	{
		jappconfig.startup_page_policy = "0";
	}
	else if($("#startup_page_policy_1").parent().is(".radio-selected"))
	{	
		jappconfig.startup_page_policy = "1";
	}
	else
	{
		jappconfig.startup_page_policy = "2";	
	}


	jappconfig.ui_show_homepage = "0";
	jappconfig.ui_show_bookmark = "0";
	jappconfig.ui_show_shortcut = "0";
	jappconfig.ui_show_restore_recent = "0";

	if($("#main-page").parent().is(".check-selected"))
	{
		jappconfig.ui_show_homepage = "1"
	}
	if($("#show-bookmarks").parent().is(".check-selected"))
	{
		jappconfig.ui_show_bookmark = "1";
	}
	if($("#screenshot").parent().is(".check-selected"))
	{
		jappconfig.ui_show_shortcut = "1";	
	}
	if($("#Restore").parent().is(".check-selected"))
	{
		jappconfig.ui_show_restore_recent = "1";	
	}




	//page label setting(书签设置)
	//

	jappconfig.tabset_qiantaitiaozhuan = "0";
	jappconfig.tabset_close_dblclick = "0";
	jappconfig.tabset_close_rightclick = "0";
	jappconfig.tabset_quit_whencloselast = "0";


	if($("#Reception").parent().is(".radio-selected")) jappconfig.tabset_qiantaitiaozhuan = "1";
	if($("#double-click-off").parent().is(".check-selected"))  jappconfig.tabset_close_dblclick = "1";	
	if($("#right-click-off").parent().is(".check-selected"))jappconfig.tabset_close_rightclick = "1";	
	if($("#close-browser").parent().is(".check-selected")) jappconfig.tabset_quit_whencloselast = "1";;	



	jappconfig.tabset_newtab_whenclickbookmark = "0";
	jappconfig.tabset_newtab_navigateaddress = "0";
	jappconfig.tabset_newtab_position = "0";
	jappconfig.tabset_activepos_whenclosetab = "0";

	if($("#opennewpage").parent().is(".radio-selected")) jappconfig.tabset_newtab_whenclickbookmark = "1";
	
	if($("#opennewpage2").parent().is(".radio-selected")) jappconfig.tabset_newtab_navigateaddress = "1";;

	if($("#current-right-side").parent().is(".radio-selected")) jappconfig.tabset_newtab_position = "1";;

	if($("#Activate-left").parent().is(".radio-selected")) jappconfig.tabset_activepos_whenclosetab = "1";
	

	//下载页面


	var download_path = $("#downloadLocationPath").attr("value");
	if(download_path.length)
	{
		jappconfig.dnload_location = download_path;
	}
	
	jappconfig.dnload_ask_befroe_newtask = "0";
	jappconfig.dnload_info_whenover = "0";

	if($("#download-before").parent().is(".check-selected"))  jappconfig.dnload_ask_befroe_newtask = "1";;
	if($("#download-complate").parent().is(".check-selected"))  jappconfig.dnload_info_whenover = "1";;


	//截图设置


	if($("#png-format").parent().addClass(".radio-selected"))
		jappconfig.shortcut_ext = "png";
	else jappconfig.shortcut_ext = "jpg";
	

	var download_path2 = $("#downloadLocationPath2").attr("value");
	if(download_path2.length)
	{
		jappconfig.location = download_path2;
	}
	
	bs.Sync(JSON.stringify(jconfig));
	setTimeout(function(){sync_settings(jconfig);},300);
}

$(function(){
	//设置菜单
	//
	//

	bs = new BrowserSetting();
    var config = bs.Query();
    var jconfig = eval('(' + config + ')');


    try {
     loading_settings(jconfig.appconfig);
    } catch (e) {
     alert(e.name + ": " + e.message);
    } 
    

    

    setTimeout(function(){sync_settings(jconfig);},300);

	




	$(".setting-menu ul li").click(function(){
		var self = $(this);
		var $index = self.index();
		self.addClass("select").siblings().removeClass("select");
		$("title").html("选项-"+self.html());
		$(".page").eq($index).show().siblings().hide();
		$(".ripple").remove();
	})
	//单项选择(radio)
	$(".span-radio input").click(function(){
		var self = $(this);
		var $name = $(this).attr("name");
		var val = $('input:radio[name='+$name+']:checked').val();
		$('input:radio[name='+$name+']').parent().removeClass("radio-selected");
		self.parent().addClass("radio-selected");
	});
	//多项选择(checkbox)
	$(".span-check input").click(function(){
		var self = $(this);
		if (self.attr("checked")) {
			self.attr("checked",false);
			self.parent().removeClass("check-selected");
		}else{
			self.attr("checked",true);
			self.parent().addClass("check-selected");
		}
	})
	//搜索框的下拉
	$(".select-row").click(function(e){
		if($(".select-ul").css('display')=="block"){
			$(".select-ul").slideUp(300);
		}else{
			$(".select-ul").slideDown(300);
		};
		return false;
	});
	//选择搜索
	$(".select-ul").on("click","li",function(e){
		var $Name = $(this).attr("gm-data");
		var $Name2 = $(".select-name").html();
		if($Name2!=$Name){
			$(".select-name").html($Name);
			$(".select-tit").removeClass("no-tit");
			$(this).find(".select-tit").addClass("no-tit");
			$(".searchHover").removeClass("defaultEngines");
			for(var t in $(".searchHover")){
				if($(".searchHover").eq(t).find(".searchName").html()==$Name){
					$(".searchHover").eq(t).addClass("defaultEngines");
				}
			}
		}else{
			$(".select-ul").slideUp(300);
			return false;
		}
	});
	$(window).click(function(e){
		//下拉框消失
		if($(".select-row").has(e.target).length === 0){
			$(".select-ul").slideUp(300);
		}
	});
	//启动页
	$("#set-userpage").click(function(){		
		$("#settingWeb-title").text("设置启动页");
		$("#settingWeb-title").data("page","0");


		if(jconfig.appconfig.user_startpage.length)
		{
			$(".settingWeb-container input").val(jconfig.appconfig.user_startpage);	
		}

		$(".settingWeb").css({zoom:"50%"});
		$(".settingWeb").show();
		$(".settingWeb").animate({zoom:"100%"},50);
	});

	$("#set-mainpage").click(function(){
		$("#settingWeb-title").text("设置主页");
		$("#settingWeb-title").data("page","1");

		if(jconfig.appconfig.main_page.length)
		{
			$(".settingWeb-container input").val( jconfig.appconfig.main_page);	
		}


		

		$(".settingWeb").css({zoom:"50%"});
		$(".settingWeb").show();
		$(".settingWeb").animate({zoom:"100%"},50);
	});


	$("#settingWeb-ok").click(function(){
		var valuetext = $(".settingWeb-container input").val();
		var pagetype  = $("#settingWeb-title").data("page");



		if(IsURL(valuetext) == false)
		{
			alert("请输入正确的url");
			return ;
		}

		if(pagetype == "0")
		{
			jconfig.appconfig.user_startpage = valuetext;

		}
		else if(pagetype == "1")
		{

			jconfig.appconfig.main_page = valuetext;
		}

		close("settingWeb");

	});


	$(".settingWeb-close").click(function(){


		close("settingWeb");

	});


	//管理搜索引擎
	$("#manage-default-search-engines").click(function(){
		$(".manageEngines").css({zoom:"50%"});
		$(".manageEngines").show();
		$(".manageEngines").animate({zoom:"100%"},50);
	});
	$(".searchBox").on("mouseenter",".searchHover",function(){
		$(this).css({background:"#ddd"});
		if($(this).hasClass("defaultEngines")){
			$(this).find("span").hide();
		}else{
			$(this).find("span").show();
			$(this).find(".searchWeb").css({width:"160px"});
			$(this).find(".setDefault").css({width:"48px"});
		};
	});
	$(".searchBox").on("mouseleave",".searchHover",function(){
		$(this).css({background:"#fff"});
		$(this).find("span").hide();
		$(this).find(".searchWeb").css({width:"208px"});
	})
	$(".searchBox").on("click",".setDefault",function(){
		$(this).parent().find("span").hide();
		$(this).parent().find(".searchWeb").css({width:"208px"});
		$(".searchHover").removeClass("defaultEngines");
		$(this).parent().addClass("defaultEngines");
		var enginesName = $(".defaultEngines").find(".searchName").html();
		$("#default-search-engine").html(enginesName);
		for(var t in $(".select-ul li")){
			if($(".select-ul li").eq(t).attr("gm-data")==enginesName){
				$(this).find(".select-tit").addClass("no-tit");
			}
		}
	})
	function close(classname){
		$("."+classname).animate({zoom:"50%"},50,function(){$("."+classname).hide();$(".ripple").remove();})
	}

	$(".settingWeb-close2").click(function(){
		close("manageEngines");
	});

	//添加默认引擎
	$(".complete").click(function(){
		var name=$(".addSearchName").val();
		var key=$(".addSearchKey").val();
		var web=$(".addsearchWeb").val();


		var conf = "[";

		for(var i = 0; i <   $(".searchBox").children().size(); ++i)
		{
			if(i != 0)
			{
				conf += ",";
			}

			var title = $(".searchBox").children().eq(i).find(".searchName").text();
			var site  = $(".searchBox").children().eq(i).find(".searchKey").text();
			var query = $(".searchBox").children().eq(i).find(".searchWeb").text();
			var def = 0;
			if($(".searchBox").children().eq(i).is(".defaultEngines"))
			{
				def = 1;
			}	

			var item = "[\"" + title +"\"," + "\"" + site +"\"," + "\"" + query +"\"," + def + "]";

			conf +=item;

		}

		conf += "]";




		jconfig.appconfig.search_engines = eval('(' + conf + ')');

		alert(jconfig.appconfig.search_engines[0][3]);




		$(".searchBox2 input").val("");
		if($(".searchBox").length<5&&name!=""&&key!=""&&web!=""){
			$(".searchBox").append('<li class="searchHover"><p class="searchName">'+name+'</p><p class="searchKey">'+key+'</p><p class="searchWeb">'+web+'</p><span class="setDefault">设为默认</span><span class="cutWeb"></span></li>')
			$(".select-ul").append('<li gm-data='+name+'><span class="engine-name">'+name+'</span><span class="select-tit">设置为默认搜索引擎</span></li>')
		}

		close("manageEngines");
	});

	//删除
	$(".searchBox").on("click",".cutWeb",function(){
		var name = $(this).parent().find(".searchName").html();
		$(this).parent().remove();
		for(var t in $(".select-ul li")){
			if($(".select-ul li").eq(t).attr("gm-data")==name){
				$(".select-ul li").eq(t).remove();
			}
		}
	})
})
