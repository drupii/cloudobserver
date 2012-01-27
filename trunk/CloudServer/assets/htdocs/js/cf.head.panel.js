$(document).ready(function() {
	$('body').append("<div class=\"hidden-c\" style=\"display: none;\"><!-- --><div id=\"tabs-logout\" class=\"logout\"><fieldset><legend>Logout?</legend><form  action=\"./ufs.service\"><input type=\"hidden\" name=\"redirect_to\" value=\""+location.href+"\"><input type=\"hidden\" name=\"user_control\" value=\"logout\"><input type=\"hidden\" name=\"logout\" value=\"true\"><input type=\"submit\" class=\"eButton\" value=\"OK\" /><input type=\"button\" class=\"eButton\" value=\"Cancel\" onClick=\'$(\".alert\").hide();\' /></form></fieldset></div><!-- --></div>");

	$('body').append("<div class='header'><div id='marx-user-name' > <div><a href='index.html' alt='Cloud Server Main Page'><p style='background: url(/logo-small.png); width:40px; height:40px;' class='logo'></p></a></div> <div data-template><a><p class='header-item' id='user_name' class='hidden-c' >{{user_name}}</p></a></div></div><div id='marx-services-list'><div data-template><a href='{{url}}'><p class='header-item'>{{name}}</p></a></div></div></div>");

   var servicesList = Tempo.prepare('marx-services-list').notify( function (event) {
    if ( event.type === TempoEvent.Types.RENDER_COMPLETE) { page_update.run(); }
	});
			servicesList.starting();
	
	$.getJSON("server.json", function(data) {
		    servicesList.render(data);
	});

	$('.header-item, .logo').live('mousedown', function(){
		$(this).addClass("hilight");
	}).live('mouseup', function(){
		$(this).removeClass("hilight");
	}).live('mouseover', function(){
		$(this).addClass("border");
	}).live('mouseout', function(){
		$(this).removeClass("border");
		$(this).removeClass("hilight");
	});
	
	user = readCookie('session-id');
	if (user != null) {
		var user_name = Tempo.prepare('marx-user-name').notify( function (event) {
    if ( event.type === TempoEvent.Types.RENDER_COMPLETE) { page_update.run(); }
	});
		user_name.starting();
		
		$.getJSON("ufs.service?user_name=true", function(data) {
		    user_name.render(data);
		});
		
		$("#marx-user-name").show();
		
		$("#user_name").live('mouseup', function(eventObj){
			th =  $(this);
			var elemZIndex = $(this).css('z-index', '100');
			var elemPos = $(this).offset();
	
			$(this).append('<div id="newer" class="new" style="position:absolute; left:' +( eventObj.pageX) + 'px; top:' + 0 + 'px; z-index:' + (elemZIndex + 10) + '">&nbsp;</div>');
	//( elemPos.top  + (40%(eventObj.pageY))
			$('#newer').grumble({
				text: "  <a href='#' class='logout-but'><h3>Log me out</h3></a> <br/> <a href='#' ><h3>Cancel</h3></a>    ",
				angle: (Math.random() * 50 + 130),
				distance: 3,
				showAfter: 15,
				hideAfter: false,
				//type : 'alt-',
				hasHideButton: true,
				// just shows the button
				buttonHideText: 'Pop!',
				onShow: function() {
					$('.logout-but').mouseup(function() {
						showForm($('#tabs-logout'), 'Logout form',350, 120);
					});
					th.addClass("border");
				},
				onBeginHide: function() {
					$('.border').removeClass("border");
				},
				onHide: function() {
					$("#newer").empty();
					$("#newer").detach();
					$("#newer").remove();
				}
			});
	
		}).live('mousedown', function(){
			$(this).addClass("hilight");
		}).live('mouseup', function(){
			$(this).removeClass("hilight");
		}).live('mouseover', function(){
			$(this).addClass("border");
		}).live('mouseout', function(eventObj){
			$(this).removeClass("border");
			$(this).removeClass("hilight");
		});					
	}else{
		$('#user_name').hide();
		}
});