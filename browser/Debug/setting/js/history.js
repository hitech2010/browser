$(function(){

	var hq = new HistoryQuery();
	var today = hq.Query("today");
	var yesterday = hq.Query("yesterday");
	var thisweek = hq.Query("thisweek");
	var thismonth = hq.Query("thismonth");

	//alert(res);
	//$("sy-today part-box").innerHTML = res;
	$(".part-right .sy-today ").html(today);
	$(".part-right .sy-yesterday ").html(yesterday);
	$(".part-right .sy-thisweek ").html(thisweek);
	$(".part-right .sy-thismonth ").html(thismonth);
	//alert($(".part-right .sy-today ").html();)
	//
	//
	//

	var txt = "Browser Name: " + navigator.appName + "\n";
  txt += "Browser Version: " + navigator.appVersion + "\n";
  //alert(txt);
	function history(s) {
		if(s > 0) {
			document.getElementById('ceng4').style.display = 'block';
			document.getElementById('close4').style.display = 'block';
		} else {
			document.getElementById('ceng4').style.display = 'none';
			document.getElementById('close4').style.display = 'none';
		}
	}
	$(".part h4").click(function(){
		var $parents = $(this).parent().parent();
		if(!$(this).hasClass("hide")){
			$(this).addClass("hide");
			$(this).find("i").addClass("xiala");
			$parents.find(".part-box").slideUp(100);
			return false;
		};
		$(this).removeClass("hide");
		$parents.find(".part-box").slideDown(100);
	});
	//浏览器记录划过
/*	$(".history-info").on("hover",function(){
		$(this).find("span").show()
	},function(){
		$(this).find("span").hide();
	});*/


	$(".history-content").on('mouseenter mouseleave','.history-info',function(){
		$(this).find("span").fadeToggle(0);
	});

	$(".history-content").on("click",".cut span",function(){
		$(this).parent().parent().remove();
	})
	//多项选择(checkbox)
	$(".history-content").on("click", ".span-check input",function(){
		var self = $(this);
		if (self.attr("checked")) {
			self.attr("checked",false);
			self.parent().removeClass("check-selected");
			self.parent().parent().removeClass("selected");
			items[self.data("index")] = 0;
		}else{
			self.attr("checked",true);
			self.parent().addClass("check-selected");
			self.parent().parent().addClass("selected");
			items[self.data("index")] = 1;
		}
	});
	//删除选中项
	$(".history-content").on("click","#del-select",function(){
		var list = new String();
		list +="(";
		for (index in items)
		{
			if(items[index] == 1)
			{
				list+=index;
				list+=","
			}
		}
		if(list.charAt(list.length -1) == ",")
		{
			list = list.substring(0,list.length-1);
		}
		list+=")";

		$(".part-box .selected").remove();

		hq.Delete(list);
		items=[];

	});
	//清空
	//
	$("#del-all").click(function(){
		hq.Clear();
		$(".history-info").remove();
	});
	//搜索框
	$("#history-search-name").on('input propertychange', function() {
	    
	});
	$("#history-search-name").focus(function(){
		$(".page").hide();
		$(".search-box").show();

	});
	$("#history-search-name").blur(function(){
		if($("#history-search-name").val()==""){
	    	$(".page").show();
			$(".search-box").hide();

	    }
	});

	$("#toSearch").click(function(){
		var keyword = $("#history-search-name").val();

		if(keyword == "")
		{
			return ;
		}

		var ret = hq.Search(keyword);

		$(".search-box").html(ret);

	});
})
