$(function(){

	var bq 			= new BookmarkQuery();

	var res 		= bq.Query();

	$(".bookmark-content .page").html(res);


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
    var bookmarkThis;
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
    	bookmarkThis.addClass("inputChange");
		bookmarkThis.find("input").addClass("inputCh").attr("disabled", false);
	};
	function disabledTrue() {
		bookmarkThis.removeClass("inputChange");
		bookmarkThis.find("input").removeClass("inputCh").attr("disabled", "disabled");
	};
    $(".repair").click(function(){
    	disabledFalse();
    	$(".mouseBox").hide();
    	return false;
    });
    function fixed(e){
    	var Width = $("body").width();
    	var Height= $("body").height();
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
	$("#bookmark-search-name").on('input propertychange', function() {
	    
	});
	$("#bookmark-search-name").focus(function(){
		$(".page").hide();
		$(".page2").show();
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

		var ret = bq.Search(keyword);

		$(".part-box").html(ret);

	});
})
