
    function OnSubmit()
	{
	    var keyword = $("#history-search-name").val();

		if(keyword == "")
		{
			return ;
		}


		$(".page").hide();
		$(".search-box").show();
		page = 1;

		var ret = hq.Search(keyword);

		$(".search-box").html(ret);

	}


$(function(){

	 hq = new HistoryQuery();
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

    function backtohistory(){
        if($("#history-search-name").val()==""){

			$(".page").show();
			$(".search-box").hide();
			page = 0;


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

        }
    }










	$(".history-content").on('mouseenter mouseleave','.history-info',function(){
		$(this).find("span").fadeToggle(0);
	});

	$("body").on("click",".cut span",function(){
		$(this).parent().parent().remove();
		
		var list = new String();
		list += "(";
		list += $(this).data("index");
		list += ")";
		hq.Delete(list);
	})
	//多项选择(checkbox)
	$(".history-content").on("click", ".span-check input",function(){
		var self = $(this);
		if (self.attr("checked")) {
			self.attr("checked",false);
			self.parent().removeClass("check-selected");
			if(page == 0)
			{
				self.parent().parent().removeClass("selected");
				items[self.data("index")] = 0;	
			}
			else if(page == 1)
			{
				self.parent().parent().removeClass("searchselected");
				search_items[self.data("index")] = 0;	
			}	
			
		}else{
			self.attr("checked",true);
			self.parent().addClass("check-selected");

			if(page == 0)
			{
				self.parent().parent().addClass("selected");
				items[self.data("index")] = 1;
			}
			else if(page == 1)
			{
				self.parent().parent().addClass("searchselected");
				search_items[self.data("index")] = 1;	
			}
			
		}
	});
	//删除选中项
	$(".history-box").on("click","#del-select",function(){

		var list = new String();
		list +="(";

			if(page == 0)
			{
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

			}
			else if(page == 1)
			{

				for (index in search_items)
				{
					if(search_items[index] == 1)
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

				$(".search-box .searchselected").remove();

				hq.Delete(list);


				search_items=[];

			}


			



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


	});
	$("#history-search-name").blur(function(){
		if($("#history-search-name").val()==""){
	    	$(".page").show();
			$(".search-box").hide();
			page = 0;

	    }
	});

	$("#toSearch").click(function(){
		var keyword = $("#history-search-name").val();

		if(keyword == "")
		{
			return ;
		}


		$(".page").hide();
		$(".search-box").show();
		page = 1;

		var ret = hq.Search(keyword);

		$(".search-box").html(ret);

	});



	if(!window.ScriptEngineMinorVersion() && window.addEventListener){//IE9
        $("#history-search-name").on("keyup",function(e){
            if(e.keyCode === 8){
                backtohistory();
            }
            if(e.ctrlKey && e.keyCode === 88){
                backtohistory();
            }
        })
    }else{
        $("#history-search-name").on('input propertychange', function() {
            backtohistory();
        });
    }


})
