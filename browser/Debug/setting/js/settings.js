



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



	
}

function sync_settings(jconfig)
{
	bs.Sync(jconfig);
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
	

	

	setTimeout(function(){sync_settings(JSON.stringify(jconfig));},300);

	




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
	$(".select-ul li").click(function(e){
		var $Name = $(this).attr("gm-data");
		var $Name2 = $(".select-name").html();
		if($Name2!=$Name){
			$(".select-name").html($Name);
			$(".select-tit").removeClass("no-tit");
			$(this).find(".select-tit").addClass("no-tit")
		}else{
			return false;
		}
	});
	$(window).click(function(e){
		//下拉框消失
		if($(".select-row").has(e.target).length === 0){
			$(".select-ul").slideUp(300);
		}
	});
	//
})
