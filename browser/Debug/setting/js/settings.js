$(function(){
	//设置菜单
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
