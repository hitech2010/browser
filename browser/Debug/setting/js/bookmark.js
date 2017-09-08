$(function(){
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
    	var Width = $("body").width();
    	var Height= $("body").height();
    	if(e.which==3){
	    	bookmarkThis = $(this);
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
    });
    function disabledFalse() {
		bookmarkThis.find("input").addClass("inputChange").attr("disabled", false);
	};
	function disabledTrue() {
		bookmarkThis.find("input").removeClass("inputChange").attr("disabled", "disabled");
	};
	$(".mouseBox").click(function(){
		$(".mouseBox").hide()
	})
    $(".repair").click(function(){
    	disabledFalse();
    	$(".mouseBox").hide();
    	return false;
    });
    $(document).click(function(e){
    	if(e.target.className.indexOf('inputChange')<0){
	    	disabledTrue()
    	}
    });
})
