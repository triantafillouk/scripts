// ==UserScript==
// @author         Mahmoud Al-Qudsi
// @name           YouTube5
// @description    Adds an option to view YouTube videos as HTML5.
// @namespace	   http://neosmart.net/
// @include        http://*.youtube.com/watch?*
// @include        http://youtube.com/watch?*
// @version        0.1
// ==/UserScript==

(function () {
  if (document.getElementById('download-youtube-video')) return;
 
   var embedElement = document.getElementById('watch-embed-div');
   
   if (embedElement) 
   {
     var html5Element = document.createElement('div');
     
     var html5Content = '<br /> <span id=\'html5-youtube-video\'> <a href=\''+'http://neosmart.net/YouTube5/?url=' + document.location.href + '\'>View in HTML5</a></span>';

     html5Element.innerHTML = html5Content;
     
     embedElement.appendChild(html5Element);
  }
  
})();