$(function(){
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
	$(".history-info").hover(function(){
		$(this).find("p").show();
		$(this).find("span").show();
	},function(){
		$(this).find("p").hide();
		$(this).find("span").hide();
	});
	$(".cut p").click(function(){
		$(this).parent().parent().remove();
	})
})
