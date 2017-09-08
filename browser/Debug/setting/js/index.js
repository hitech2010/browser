$(function(){
	//定义不同大小的浏览器的推荐框大小
	function call1(){
		$(".most-visited").css({width:"592px"});
		$(".gm-li").css({width:"180px",height:"120px",margin:"8.6px"});
		$("#gm-hot .gm-li:gt(2)").hide();
	}
	function call2(){
		$(".gm-li").show();
		$(".most-visited").css({width:"792px"});
		$(".gm-li").css({height:"120px"})
	}
	function call3(){
		$(".gm-li").show();
		$(".most-visited").css({width:792*$width/1024+"px"});
		$(".gm-li").css({width:180*$width/1024,height:120*$width/1024+"px"})
	}
	var $width = $(window).width();
	if($width<=890){
		call1();
	}else if(890<$width<=1024){
		call2();
	}else if($width>1024){
		call3();
	}
	$(window).resize(function(){
		$width = $(window).width();
		if($width<=890){
			call1();
		}
		if(890<$width&&$width<=1024){
			call2();
		}
		if($width>1024){
			call3();
		}
	});
	//搜索框的下拉
	$(".search-engine").click(function(e){
		if($(".engine-list").css('display')=="block"){
			$(".engine-list").fadeOut();
		}else{
			$(".engine-list").fadeIn();
		};
		return false;
	});
	//选择搜索
	$(".engine-list li").click(function(e){
		var $Name = $(this)[0].className;
		var $Name2 = $(".more").attr("gm-data");
		if($Name2!=$Name){
			$(".more").removeClass("gm-"+$Name2)
			$(".more").addClass("gm-"+$Name);
			$(".more").attr("gm-data",$Name);
		}
	});
	$(window).click(function(e){
		//下拉框消失
		if($(".search-engine").has(e.target).length === 0){
			$(".engine-list").fadeOut();
		}
	})
})
