$(function(){
	//定义不同大小的浏览器的推荐框大小
	//
	//
	
	
	function index_delete_history(index)
	{
		Ipage.Delete(index);
	}

	function index_query()
	{
		var config = Ipage.Query();
	

		var jconfig = eval('(' + config + ')');

		$("#h_img1").attr("src", jconfig.p1);	
		$("#h_img2").attr("src", jconfig.p2);
		$("#h_img3").attr("src", jconfig.p3);
		$("#h_img4").attr("src", jconfig.p4);


		$("#h_link1").attr("href", jconfig.a1);	
		$("#h_link2").attr("href", jconfig.a2);
		$("#h_link3").attr("href", jconfig.a3);
		$("#h_link4").attr("href", jconfig.a4);


		$("#h_title1").text( jconfig.t1);	
		$("#h_title2").text( jconfig.t2);
		$("#h_title3").text( jconfig.t3);
		$("#h_title4").text( jconfig.t4);



		//setTimeout(function(){index_query();},200);
	}

	Ipage = new IndexPage();
	index_query();



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
		

		if($Name == "sougou")
		{
			$("#sc").attr("action", "https://www.sogou.com/web");
			$("#query").attr("name", "query");
		} 
		else if($Name == "baidu")
		{
			$("#sc").attr("action", "https://www.baidu.com/s");
			$("#query").attr("name", "wd");
		}



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
