function OnSubmit()
{

		var keyword = $("#bookmark-search-name").val();

		if(keyword == "")
		{
			return ;
		}

		$(".page").hide();
		$(".page2").show();

		var ret = bq.Search(keyword);

		$(".part-box").html(ret);
}



$(function(){
	var bookmarkThis;

	bq 			= new BookmarkQuery();

	var res 		= bq.Query();


		function _show(cls,data)  
    {  
    	

         $(".bookmark-content  "+ cls).html(data);
    } 
    
    
    
    setTimeout(function(){_show(".page",res) }, 100)





	$("#ImportBookmarks").click(function(){

			bq.Import();

	});
	

	$("#ExportBookmarks").click(function(){

			bq.Export();

	});

	$(".history-info").click(function(){



			var urlnode = $(this).children(".bookmark-src-div");

			var hasinput = urlnode.children().size();

			if(hasinput == false)
			{
				var url = urlnode.text();

				bq.Open(url);
			}



	});
	function backtobookmark(){
		if($("#bookmark-search-name").val()==""){

			var res 		= bq.Query();




			$(".bookmark-content .page").html(res);

			$(".page").show();
			$(".page2").hide();
		}
	}

	$("#menu_newtab").click(function(){


			$(".mouseBox").hide();

			var urlnode = bookmarkThis.children(".bookmark-src-div");
			var hasinput = urlnode.children().size();



			if(hasinput == false)
			{
				var url = urlnode.text();

				bq.Open(url);
			}

	});



	$(".part h4").click(function(){
		var $parents = $(this).parent().parent();
		if(!$(this).hasClass("hide")){
			$(this).addClass("hide");
			$(this).find("i").addClass("xiala");
			$parents.find(".part-down").slideUp(100);
			$parents.addClass("part-border");
			return false;
		};
		$(this).removeClass("hide");
		$parents.find(".part-down").slideDown(100);
		$parents.removeClass("part-border");
	});
	//鼠标右键
	$(document).on("contextmenu",function(e){
        return false;
    });
    
    $(".history-info").mousedown(function(e){
    	var event = e;
    	if(event.which==3){
	    	if(bookmarkThis!==undefined){
		    	disabledTrue();
	    	}
	    	bookmarkThis = $(this);
    		fixed(event);
    	}
    });
    function disabledFalse() {


		var name= bookmarkThis.find(".bookmark-name-div").html().replace(/(^\s*)|(\s*$)/g, "");
		var src = bookmarkThis.find(".bookmark-src-div").html().replace(/(^\s*)|(\s*$)/g, "");
		bookmarkThis.find(".bookmark-name-div").html("<input class='bookmark-name inputCh' type='text' value='"+name+"'>");
		bookmarkThis.find(".bookmark-src-div").html("<input class='bookmark-src inputCh' type='text' value='"+src+"'>");
		
    	//bookmarkThis.addClass("inputChange");
		//bookmarkThis.find("input").addClass("inputCh").attr("disabled", false);
	};
	function disabledTrue() {
		var name= bookmarkThis.find(".bookmark-name").val();
		var src = bookmarkThis.find(".bookmark-src").val();




		if(name != undefined && src != undefined)
		{
			var id = bookmarkThis.find(".bookmark-name-div").data("index");
			if(id != undefined )
			{
				bq.Edit(id+"", name, src);
			}

				bookmarkThis.find(".bookmark-name-div").html(name);
				bookmarkThis.find(".bookmark-src-div").html(src);
		}


		
		//bookmarkThis.removeClass("inputChange");
		//bookmarkThis.find("input").removeClass("inputCh").attr("disabled", "disabled");
	};
    $(".repair").click(function(){
    	disabledFalse();
    	$(".mouseBox").hide();
    	return false;
    });
    function fixed(e){
    	var Width = $(window).width();
    	var Height= $(window).height();
    	var mouseHeight = $(".mouseBox").height();
		$(".mouseBox").show();	
		if((e.clientX+220)>Width){
			if((mouseHeight+e.clientY)>Height){
				$(".mouseBox").css({top:Height-mouseHeight+"px",left:(e.clientX-180)+"px"});
			}else{
    			$(".mouseBox").css({top:e.clientY+"px",left:(e.clientX-180)+"px"});
			}
		}else{
			if((mouseHeight+e.clientY)>Height){
				$(".mouseBox").css({top:Height-mouseHeight+"px",left:e.clientX+"px"});
			}else{
	    		$(".mouseBox").css({top:e.clientY+"px",left:e.clientX+"px"})
			}
		}
    }
    //右击事件
    $(document).mousedown(function(e){
    	if(e.which==3){
    		if($(".history-info").has(e.target).length === 0){
    		}
    	}
    });
    $(document).click(function(e){
    	if($(".mouseBox").has(e.target).length === 0){
    		$(".mouseBox").hide();
	    	if(e.target.className.indexOf('inputCh')<0){
		    	disabledTrue()
	    	};
    	}
    	e.preventDefault();
    });
    //搜索框
	
	$("#bookmark-search-name").focus(function(){

	});
	$("#bookmark-search-name").blur(function(){
		if($(this).val()==""){
			$(".page").show();
			$(".page2").hide();
		}
	});


	$("#toSearch").click(function(){
		var keyword = $("#bookmark-search-name").val();

		if(keyword == "")
		{
			return ;
		}

		$(".page").hide();
		$(".page2").show();

		var ret = bq.Search(keyword);

		$(".part-box").html(ret);

	});




	if(!window.ScriptEngineMinorVersion() && window.addEventListener){//IE9
		$("#bookmark-search-name").on("keyup",function(e){
			if(e.keyCode === 8){
				backtobookmark();
			}
			if(e.ctrlKey && e.keyCode === 88){
				backtobookmark();
			}
		})
	}else{
		$("#bookmark-search-name").on('input propertychange', function() {
			backtobookmark();
		});
	}
})
