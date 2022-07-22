var almsamim = {};
// objects
var comm, bbcode;
// vars
var isIE, isOpera,isGecko, isWebKit,ua,Editor;
// Browsers check
ua = navigator.userAgent;
almsamim.isOpera = isOpera = window['opera'] && opera.buildNumber;
almsamim.isWebKit = isWebKit = /WebKit/.test(ua);
almsamim.isGecko = isGecko = !isWebKit && /Gecko/.test(ua);
almsamim.isIE = isIE = !isWebKit && !isOpera && (/MSIE/gi).test(ua) && (/Explorer/gi).test(navigator.appName);
// ----------- almsamim plug-ins object -----------//
almsamim.plugins = {};
//thanks tuantrinh  smilies replace with bbocde
if (typeof window['smiley_wysiwyg'] == 'undefined')
{
       var smiley_wysiwyg = false;
}
if (smiley_wysiwyg)
{
       var smiley_code_bbcode = new Array();
       var smiley_img_bbcode = new Array();
       var t = document.URL;
       t =   t.replace("http://", "");
       var u = t.split("/");
       var v = "http://";
       for (var d = 0; d < u.length; d++) {
          if (d < u.length - 1) {
             v += u[d] + "/";
          }
       }

       for(var i = 0; i < smiley_total; i++) {
          smiley_code_bbcode[i] = smiley_code[i];
          smiley_img_bbcode[i] = "[IMG]" + v + smiley_path + smiley_img[i] + "[/IMG]";
       }

       function Smiley_BBcodetoHTML(str) {
          for(var i = 0; i < smiley_total; i++) {
             while (str.indexOf(smiley_code_bbcode[i]) > -1) {
                str = str.replace(smiley_code_bbcode[i], smiley_img_bbcode[i]);
             }
          }
          return str;
       };

       function Smiley_HTMLtoBBcode(str) {
          for(var i = 0; i < smiley_total; i++) {
             while (str.indexOf(smiley_img_bbcode[i]) > -1) {
                str = str.replace(smiley_img_bbcode[i], smiley_code_bbcode[i]);
             }
          }
          return str;
       };
}

// ----------- almsamim bbcode object -----------//
almsamim.bbcode = bbcode = {

// turning the editor on
Start  : function (){
		var tDiv = document.getElementById("dvSample");	
		Editor = document.getElementById('box').contentWindow.document;
		//writing iframe content and style of the editor
		var iframeContent;
		iframeContent  = '<html xmlns="http://www.w3.org/1999/xhtml" dir="' + lang_s[9] + '" lang="en-gb" xml:lang="en-gb">\n';
		iframeContent += '<head><style type="text/css">body{font-family:Verdana,Arial,Sans-Serif,Tahoma;font-size:12px;background:'+ lang_s[6] +';color:' + lang_s[7] + ';margin:2px;padding:2px;}p {margin:0;padding:0;min-height: 1em; } *+html p { min-height: auto; }div {margin:0;padding:0;}</style></head><body>';	
		if (tDiv.value !== ""){
		var v = tDiv.value;
		iframeContent += this._BBcodetoHTML(v) ;
		}
		iframeContent += '</body>';
		iframeContent += '</html>';
		Editor.open();
		Editor.write(iframeContent);
		Editor.close();
		if(isIE){
			Editor.attachEvent('onkeypress', editorEvents);
			Editor.attachEvent('onmousedown', editorEvents);
			Editor.attachEvent('onmousedown', hideall);
			Editor.body.contentEditable = true;
		}else{
			Editor.addEventListener('keypress', editorEvents, true);
			Editor.addEventListener('mousedown', editorEvents, true);
			Editor.addEventListener('mousedown', hideall, true);
			Editor.designMode = "on";
		}
		if(!isWebKit){
		// disable CSS in Geko ,IE and opera  
		try {
			// Try new Gecko method
			Editor.execCommand("styleWithCSS", 0, false);
		} catch (e) {
			// Use old method
			try {Editor.execCommand("useCSS", 0, true);} catch (e) {};
		}
		}
},
// convert BBcode to HTML code
_BBcodetoHTML : function (a)
{
    function r(re, str) {
            a = a.replace(re, str);
    };
	   if (smiley_wysiwyg)   {
          a=Smiley_BBcodetoHTML(a);
       }
	r(/\n+(\[\/list\])/gi,'[/list]');
	r(/\[list\]\n+/gi,'[list]');
	r(/\[list=1\]\n+/gi,'[list=1]');
	r(/\[list=a\]\n+/gi,'[list=a]');
	r(/\n+\[\/tr\]/gi,'[/tr]');
	r(/\n+\[tr\]/gi,'[tr]');
	r(/\n+\[td\]/gi,'[td]');
	r(/\n+\[\/td\]/gi,'[/td]');
	r(/\n+\[\/table\]/gi,'[/table]');
	r(/\[\/table\]$/gi,"[/table]\n");
	r(/&/g,'&amp;');
	r(/</g,'&lt;');
	r(/>/g,'&gt;');
	r(/  /g,'&nbsp;&nbsp;');
	r(/\t/g,'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;');	
	r(/\n/g,'<br>');
	r(/\[hr\]/gi,'<hr />');
	r(/\[\/hr]/gi,'');
//	r(/\[table\]/gi,'<table style="width: 100%; padding: 0px;border:1px dotted gray;">');
	r(/\[table\]/gi,'<table style="width: 100%; border-collapse:collapse ; padding: 1px;border:2px solid gray;">');
	r(/\[\/table\]/gi,'</table>');
	r(/\[(\/|)tr\]/gi,'<$1tr>');
//	r(/\[(\/|)td\]/gi,'<$1td style="font-size: 20px; display:table-cell;padding:1px;vertical-align:inherit;border:1px dotted #CCCCCC;">');
	r(/\[(\/|)td\]/gi,'<$1td style=" display:table-cell;padding:1px;vertical-align:inherit;border:1px solid #FFFFCC;">');
	r(/\[(\/|)indent\]/gi,'<$1blockquote>');
	r(/\[(sub|sup|strike|s|b|i|u|blockquote)\]/gi,'<$1>');
	r(/\[\/(sub|sup|strike|s|b|i|u|blockquote)\]/gi,'</$1>');
	r(/\[font=(.*?)\]/gi,'<font face="$1">');
	r(/\[color=(.*?)\]/gi,'<font color="$1">');
	r(/\[highlight=(.*?)\]/gi,'<span style="background-color:$1">');
	r(/\[\/highlight\]/gi,'</span>');	
	r(/\[size=(.*?)\]/gi,'<font size="$1">');
	r(/\[\/(font|color|size)\]/gi,'</font>');
	r(/\[(center|left|right|justify)\]/gi,'<div align="$1">');
	r(/\[\/(center|left|right|justify)\]/gi,'</div>');
	r(/\[url=(.*?)\]/gi,'<a href="$1">');
	r(/\[url\](.*?)\[\/url\]/gi,'<a href="$1">$1[/url]');
	r(/\[\/url\]/gi,'</a>');
	r(/\[img\](.*?)\[\/img\]/gi,'<img src="$1">');
	var b=a.match(/\[(list|list=1|list=a)\]/gi);
	r(/\[list=1\]/gi,'<ol>');
	r(/\[list=a\]/gi,'<ol style="list-style-type: lower-alpha">');
	r(/\[list\]/gi,'<ul>');
	r(/\[\*\]/gi,'<li>');
	r(/<br[^>]*><li>/gi,'<li>');
	r(/<br[^>]*> <li>/gi,'<li>');
	r(/<br[^>]*><\/li>/gi,'</li>');
	r(/\[h([1-6])?\]/gi,"<h$1>");
	r(/\[\/h([1-6])?\]/gi,"</h$1>");
	if(b){for(var i=0;i<b.length;i++){if(b[i].toLowerCase()=="[list]"){r(/\[\/list\]/i,'</ul>');}else if(b[i].toLowerCase()=="[list=1]"||b[i].toLowerCase()=="[list=a]"){r(/\[\/list\]/i,'</ol>');}}}
	if(isOpera){r(/<\/table>/gi,'</tr></table>');r(/<\/tr>/gi,'</td></tr>');}	
	if(isOpera||isIE){r(/<li>/gi,'</li><li>');r(/<\/(ol|ul)>/gi,'</li></$1>');}
	return a;
},

// erase white spaces from the text inside the editor
_erase : function (a)
{
	if(typeof a!="string")return a;
	var b=a;
	var c=b.substring(0,1);
	while(c==" ")
	{
		b=b.substring(1,b.length);
		c=b.substring(0,1)
	}
	c=b.substring(b.length-1,b.length);
	while(c==" ")
	{
		b=b.substring(0,b.length-1);
		c=b.substring(b.length-1,b.length)
	}
	while(b.indexOf("  ")!=-1)
	{
		b=b.substring(0,b.indexOf("  "))+b.substring(b.indexOf("  ")+1,b.length)
	}
	return b ;
},

// convert HTML to bb when submit bbcode
_HTMLtoBBcode : function (a) {
	    function r(re, str) {
	            a = a.replace(re, str);
	    };
        if (isIE) {
            r(/<\/li>/gi, "");
            r(/<li>/gi, "[*]");
        }
        r(/<div><\/div>/gi, "");
        r(/<br[^>]*>/gi, "<br>");
        r(/[\n\r]/gi, " ");
        r(/<script>(.*?)<\/script>/gi, "");
        r(/<script.*?>(.*?)<\/script>/gi, "");
		// remove style tag with anything within
        r(/<style>(.*?)<\/style>/gi, "");
        r(/<style.*?>(.*?)<\/style>/gi, "");
		// remove tags <w:> of MS word
        r(/<w:.*?>(.*?)<\/w:.*?>/gi, "");
        r(/<(abbr|acronym|applet|area|base|basefont|bdo|bgSound|big|body|button|caption|center|cite|code|col|colGroup|comment|custom|dd|del|dfn|dir|dl|dt|embed|fieldSet|frame|frameSet|head|html|ins|isIndex|kbd|label|legend|link|listing|map|marquee|menu|meta|noBR|noFrames|noScript|optGroup|option|param|plainText|pre|q|rt|ruby|samp|small|tBody|tFoot|tHead|title|tt|wbr|xml|xmp|th|script|form|input|iframe|object|select|textarea)(.*?)>/gi, "");
        r(/<\/(abbr|acronym|applet|area|base|basefont|bdo|bgSound|big|body|button|caption|center|cite|code|col|colGroup|comment|custom|dd|del|dfn|dir|dl|dt|embed|fieldSet|frame|frameSet|head|html|ins|isIndex|kbd|label|legend|link|listing|map|marquee|menu|meta|noBR|noFrames|noScript|optGroup|option|param|plainText|pre|q|rt|ruby|samp|small|tBody|tFoot|tHead|title|tt|wbr|xml|xmp|th|script|form|iframe|object|select|textarea)(.*?)>/gi, "");
        r(/\xA0/gi, " ");
        r(/<br[^>]*><\/div>/gi, "</div>");
        r(/<br[^>]*>/gi, "\n");
        r(/<hr[^>]*>/gi, "[hr][/hr]");
        r(/<\/hr>/gi, "");
        r(/<(ul|ol)><\/li>/gi, "<$1>");
        r(/  /gi, " ");
        r(/<p([^>]*)>/gi, "<div$1>");
        r(/<\/p([^>]*)>/gi, "</div$1>\n");
        r(/\t/g, "     ");
        r(/\n /g, "\n");
        r(/<a.*?href=\"(.*?)\".*?>(.*?)<\/a>/gi, "[url=$1]$2[/url]");
        r(/<h([1-6])([^>]*)>/gi, "[h$1]");
        r(/<\/h([1-6])([^>]*)>/gi, "[/h$1]");
        var b = a.split("<");
        var c = new Array;
        var e = 0;
        if (b.length > 1) {
            for (var i = 0; i < b.length; i++) {
                if (i > 0) {
                    b[i] = "<" + b[i];
                }
                var f = b[i];
                if (f.match(/<(div|span|font|strong|b|u|i|em|var|address|h1|h2|h3|h4|h5|h6|blockquote|img|ol|ul|li|a|strike|s|sub|sup|hr|table|tr|td)( ([^>]{1,}.*?)){0,1}( {0,1}){0,1}>/i)) {
                    var g = RegExp.$1;
                    var h = RegExp.$3;
                    if (h.toLowerCase().indexOf("style=") != -1 && h.toLowerCase().indexOf("font-family:") != -1 && h.toLowerCase().indexOf("face=") != -1) {
                        h = h.replace(/face="(.*?)"/gi, "");
                    } else if (h.toLowerCase().indexOf("style=") != -1 &&  h.toLowerCase().indexOf("color:") != -1 && h.toLowerCase().indexOf("color=") != -1) {
                        h = h.replace(/color="(.*?)"/gi, "");
                    }
                    h = h.replace(/(color=|size=|face=|style=)/gi, "|$1");
                    h = h.replace(/('|")/g, "");
                    h = h.replace(/ \|/g, "|");
                    var j = h.split("|");
                    var k = new Array;
                    if (j != null) {
                        for (var z = 0; z < j.length; z++) {
                            var l = j[z].split("=");
                            k[l[0].toLowerCase()] = j[z].replace(l[0].toLowerCase() + "=", "");
                        }
                    }
                    var m = "";
                    var g = g.toLowerCase();
                    if (g == "strike" || g == "s") {
                        if (k['style']) {
                            m = "[s]" + this._process(g, k);
                        } else {
                            m = "[s]";
                        }
                    } else if (g == "sub") {
                        if (k['style']) {
                            m = "[sub]" + this._process(g, k);
                        } else {
                            m = "[sub]";
                        }
                    } else if (g == "sup") {
                        if (k['style']) {
                            m = "[sup]" + this._process(g, k);
                        } else {
                            m = "[sup]";
                        }
                    }else if(g=="blockquote"){
					if(k['style'])
						{
						m = '[blockquote]'+this._process(g,k);
					}
					else
						{
						m='[blockquote]';
					}
				} else if (g == "li") {
                        if (k['style']) {
                            m = "[*]" + this._process(g, k);
                        } else {
                            m = "[*]";
                        }
                    } else if (g == "strong" || g == "b") {
                        if (k['style']) {
                            if (k['style'].toLowerCase().indexOf("font-weight: bold") != -1 ||  k['style'].toLowerCase().indexOf("font-weight: 700") != -1) {
                                m = this._process(g, k);
                            } else {
                                m = "[b]" + this._process(g, k);
                            }
                        } else {
                            m = "[b]";
                        }
                    } else if (g == "em" ||
                        g == "i" || g == "var" || g == "address") {
                        if (k['style']) {
                            if (k['style'].toLowerCase().indexOf("font-style: italic") != -1) {
                                m = this._process(g, k);
                            } else {
                                m = "[i]" + this._process(g, k);
                            }
                        } else {
                            m = "[i]";
                        }
                    } else if (g == "u") {
                        if (k['style']) {
                            if (k['style'].toLowerCase().indexOf("text-decoration: underline") != -1) {
                                m = this._process(g, k);
                            } else {
                                m = "[u]" + this._process(g, k);
                            }
                        } else {
                            m = "[u]";
                        }
                    } else if (g == "ol") {
                        if (k['style']) {
                            m = this._process(g, k);
                            if (m.indexOf("[list=a]") == -1) {
                                m += "[list=1]";
                            }
                        } else if (k.align) {
                            m = "[" + k.align.toUpperCase() + "]" + "[list=1]";
                        } else {
                            m = "[list=1]";
                        }
                    } else if (g == "ul") {
                        if (k['style']) {
                            m = this._process(g, k) + "[list]";
                        } else if (k.align) {
                            m = "[" + k.align.toUpperCase() + "]" + "[list=1]";
                        } else {
                            m = "[list]";
                        }
                    } else if (g == "font" || g == "h1" || g == "h2" || g == "h3" || g == "h4" || g == "h5" || g == "h6") {
                        if (j.length > 0) {
                            for (var r in k) {
                                if (r == "color") {
                                    m += "[color=" + k.color + "]";
                                } else if (r == "size") {
                                    if (isNaN(parseInt(k.size))) {
                                        k.size = 2;
                                    }
                                    m += "[size=" + k.size + "]";
                                } else if (r == "face") {
                                    m += "[font=" + k.face + "]";
                                } else if (r == "style") {
                                    m += this._process(g, k);
                                }
                            }
                        }
                    } else if (g == "div" || g == "span") {
                        if (k['style']) {
                            m = this._process(g, k);
                        } else if (k.align) {
                            m = "[" + k.align.toUpperCase() + "]";
                        } else {
                            m = "[ALMSAMIM]";
                        }
                    } else if (g == "img") {
                        if (isWebKit) {
                            f = f.replace(/<img(.*?)src="(.*?)">/gi, "[IMG]$2[/IMG]");
                        } else {
                            f.match(/<img(.*?)src="(.*?)"(.*?)>/gi);
                            var s = RegExp.$2;
                            s = s.replace("./", "");
                            if (s.toLowerCase().substr(0, 7) != "http://") {
                                var t = document.URL;
                                t = t.replace("http://", "");
                                var u = t.split("/");
                                var v = "http://";
                                for (var d = 0; d < u.length; d++) {
                                    if (d < u.length - 1) {
                                        v += u[d] + "/";
                                    }
                                }
                                f = f.replace(/\<img(.*?)src="(.*?)"(.*?)>/gi, "[IMG]" + v + s + "[/IMG]");
                            } else {
                                f = f.replace(/<img(.*?)src="(.*?)"(.*?)>/gi, "[IMG]$2[/IMG]");
                            }
                        }
                    } else if (g == "table") {
                        m = "[table]";
                    } else if (g == "tr") {
                        m = "[tr]";
                    } else if (g == "td") {
                        m = "[td]";
                    }
                    b[i] = f.replace(/(<([^>]+)>)/, m);
                    if (g != "img") {
                        c[e] = m;
                        e++;
                    }
                } else if (f.match(/<\/(div|span|font|strong|b|u|i|em|var|address|h1|h2|h3|h4|h5|h6|blockquote|ol|ul|li|a|strike|s|sub|sup|table|tr|td)>/i)) {
                    e--;
                    var w = c.pop();
                    if (w != null) {
                        var x = "";
                        var A = w;
                        A = A.replace(/=(.*?)\]/g, "]");
                        A = A.replace(/\]/g, "],");
                        A = A.replace(/\[(.*?)\]/g, "[/$1]");
                        var B = A.split(",");
                        B.reverse();
                        for (var y = 0; y < B.length; y++) {
                            x += B[y];
                        }
                        x = x.replace(/\[\/\*\]/gi, "");
                        b[i] = b[i].replace(/(<([^>]+)>)/, x);
                    } else {
                        b[i] = b[i].replace(/(<([^>]+)>)/, "");
                    }
                }
            }
            var C = b.join("");
        } else {
            var C = a;
        }
		function r2(re, str) {
	            C = C.replace(re, str);
	    };
        r2(/<[^>]*>/g, "");
        r2(/&lt;/g, "<");
        r2(/&gt;/g, ">");
        r2(/&nbsp;/g, " ");
        r2(/&amp;/g, "&");
        r2(/     /g, "\t");
        r2(/\[ALMSAMIM\]/g, "\n");
        r2(/\[ALMSAMIM\]\n+/g, "\n");
        r2(/\[\/ALMSAMIM\]\n+/g, "\n");
        r2(/\[\/ALMSAMIM\]/g, "\n");
        r2(/\[\*\]/gi, "\n[*]");
        r2(/\n\n\[\*\]/gi, "\n[*]");
        r2(/\[color=#.\w*\]\[\/color\]/gi, "");
		r2(/\[HIGHLIGHT=#.\w*\]\[\/HIGHLIGHT\]/gi,"");		
        r2(/\[size=\d\]\[\/size\]/gi, "");
        r2(/\[b\]\[\/b\]/gi, "");
        r2(/\[u\]\[\/u\]/gi, "");
        r2(/\[i\]\[\/i\]/gi, "");
        r2(/\[left\]\[\/left\]/gi, "");
        r2(/\[center\]\[\/center\]/gi, "");
        r2(/\[right\]\[\/right\]/gi, "");
        r2(/\[justify\]\[\/justify\]/gi, "");
        r2(/\[blockquote\]\[\/blockquote\]/gi, "");
        r2(/\[url\]\[\/url\]/gi, "");
        r2(/\[s\]\[\/s\]/gi, "");
        r2(/\[sub\]\[\/sub\]/gi, "");
        r2(/\[sup\]\[\/sup\]/gi, "");
        r2(/\[img\]\[\/img\]/gi, "");
        r2(/^\n+/, "");
        r2(/\n+$/, "");
        var D = C.match(/\[table\]/gi);
        var E = C.match(/\[\/table\]/gi);
        if (D && E) {
            if (D.length > E.length) {
                C += "[/table]";
            }
        } else if (D && !E) {
            C += "[/table]";
        }
        r2(/\[\/tr\]/gi, "\n[/tr]");
        r2(/\[tr\]/gi, "\n[tr]");
        r2(/\[td\]/gi, "\n[td]");
        r2(/\[\/table\]/gi, "\n[/table]");
        r2(/\[\/table\]$/gi, "[/table]\n");
        r2(/\[table\]\n+/gi, "[table]");
        r2(/\n+\[td\]/gi, "[td]");
        r2(/\n+\[\/table\]/gi, "[/table]");
        r2(/\n+\[\/td\]/gi, "[/td]");
        r2(/\n+\[tr\]/gi, "[tr]");
        r2(/\n+\[\/tr\]/gi, "[/tr]");
		if (smiley_wysiwyg){
             C=Smiley_HTMLtoBBcode(C);
        }
        return C;
},

// processing HTML code to BBcode
_process : function (a, b) {
	var c="";
	var d=b['style'].split(";");
	for(var j=0;j<d.length;j++){
		if(d[j]!=""&&d[j]!=null){
			var e=d[j].split(":");
			var f=e[0].toLowerCase().replace(/ /g,"");
			f=f.replace(/style=/gi,"");
			var g=e[1].replace(/^ +| +$/g,"");
			if(f=="background-color")
				{
					if(g.indexOf("#")==-1)
					{
						var h=this._toHex(g);
					} else {
						var h=g ;
					}
				c+='[highlight='+h+']';
				}
				else if (f == "vertical-align" && g == "sub") {
                    c += "[sub]";
                } else if (f == "vertical-align" && g == "super") {
                    c += "[sup]";
                } else if (f == "list-style-type" && g == "lower-alpha") {
                    c += "[list=a]";
                } else if (f == "text-align") {
                    g = g.toUpperCase();
                    c += "[" + g + "]";
                } else if(f=="margin-left"||f=="margin-right"){
				g=parseInt(g)/40;
				for(var z=0;z<g;z++)
					{
					c+='[blockquote]';
					}
			}else if (f == "font-weight") {
                    if (g.toUpperCase() == "BOLD" || g.toUpperCase() == "700") {
                        c += "[b]";
                    }
                } else if (f == "font-style") {
                    if (g.toUpperCase() == "ITALIC") {
                        c += "[i]";
                    }
                } else if (f == "font-family") {
                    c += "[font=" + g + "]";
                } else if (f == "font-size") {
                    if (g == "8pt" || g == "9pt" || g == "x-small") {
                        c += "[size=1]";
                    } else if (g == "10pt" || g == "11pt" || g == "small") {
                        c += "[size=2]";
                    } else if (g == "12pt" || g == "13pt" || g == "medium") {
                        c += "[size=3]";
                    } else if (parseInt(g) >= 14 && parseInt(g) < 18 || g == "large") {
                        c += "[size=4]";
                    } else if (parseInt(g) >= 18 && parseInt(g) < 24 || g == "x-large") {
                        c += "[size=5]";
                    } else if (parseInt(g) >= 24 && parseInt(g) < 36 || g == "xx-large") {
                        c += "[size=6]";
                    } else if (parseInt(g) >= 36 || g == "-webkit-xxx-large") {
                        c += "[size=7]";
                    }
                } else if (f == "text-decoration") {
                    if (g.toUpperCase() == "UNDERLINE") {
                        c += "[u]";
                    } else if (g.toUpperCase() == "LINE-THROUGH") {
                        c += "[s]";
                    }
                } else if (f == "color") {
					g=g.replace(/^ +| +$/g,"");
                    if (g.indexOf("#") == -1) {
					if (g.match(/rgb\((.*?)\)/gi)){
					var h = this._toHex(g);
					}else{
                       var h = g; }
					   } else {
                        var h = g;
                    }
                    c += "[color=" + h + "]";
                }
            }
        }
        return c;
},
//converting rgb(..) to Hex
_toHex : function (a)
	{
	a=a.replace(/rgb\((.*?)\)/gi,"$1");
	a=a.replace(/ /,"");
	var c=a.split(",");
	var r=parseInt(c[0]).toString(16);
	var g=parseInt(c[1]).toString(16);
	var b=parseInt(c[2]).toString(16);
	if(r.length==1)r="0"+r;
	if(g.length==1)g="0"+g;
	if(b.length==1)b="0"+b;
	return"#"+r+g+b
}
};

// ----------- almsamim command object -----------//
almsamim.comm = comm = {
_viewmode : 1 ,
_focus : function ()
{
var editor = document.getElementById('box');
editor.contentWindow.focus();
},
// the execCommand function	
_command : function (command)
{
	if (this._viewmode == 1){
	this._focus();
	document.getElementById('box').contentWindow.document.queryCommandEnabled(command);
	document.getElementById('box').contentWindow.document.execCommand(command, false, null);
	this._focus();
}else {
if (command == 'bold'){
bbfontstyle('[b]','[/b]');
}else if (command == 'italic'){
bbfontstyle('[i]','[/i]');
}else if (command == 'underline'){
bbfontstyle('[u]','[/u]');
}else if (command == 'StrikeThrough'){
bbfontstyle('[s]','[/s]');
}else if (command == 'InsertHorizontalRule'){
bbfontstyle('[hr]','[/hr]');
}else if (command == 'subscript'){
bbfontstyle('[sub]','[/sub]');
}else if (command == 'superscript'){
bbfontstyle('[sup]','[/sup]');
}else if (command == 'justifyleft'){
bbfontstyle('[left]','[/left]');
}else if (command == 'justifyright'){
bbfontstyle('[right]','[/right]');
}else if (command == 'justifyfull'){
bbfontstyle('[justify]','[/justify]');
}else if (command == 'justifycenter'){
bbfontstyle('[center]','[/center]');
}else if (command == 'InsertOrderedlist'){
bbfontstyle('[list=1]','[/list]');
}else if (command == 'InsertUnOrderedlist'){
bbfontstyle('[list]','[/list]');
}else if (command == 'indent'){
bbfontstyle('[blockquote]','[/blockquote]');
}else {
alert(lang_s[17]);
}
}
},

// create URL of selection through prompt
_url : function (prompturl) 
{
	if (this._viewmode == 1){
	if (prompturl == 1){
		var szURL = prompt(lang_s[12], "http://");
		var tURL = prompt(lang_s[8], "");
		if(tURL){
		this._HTML(' ',' ','<a href ="'+ szURL +'">'+ tURL +'</a>');
		}
	}else{ 
		var szURL = prompt(lang_s[12], "http://");
		if ((szURL != null) && (szURL != "")) {
			var Editor = document.getElementById('box').contentWindow.document;
			this._focus();
			Editor.queryCommandEnabled("CreateLink");
			Editor.execCommand("CreateLink", false,szURL);
			this._focus();	
		  }
	  }
	  }else {
	  if (prompturl == 1){
		var szURL = prompt(lang_s[12], "http://");
		var tURL = prompt(lang_s[8], "");
bbfontstyle('[url=' + szURL +']' + tURL ,'[/url]');
}else{
bbfontstyle('[url=]','[/url]');
}
}
},
// select font size,font-family,heading
_select : function (selection,selected)
{
 if (this._viewmode == 1){
    var Editor = document.getElementById('box').contentWindow.document;
	this._focus();
	Editor.queryCommandEnabled(selection);
	Editor.execCommand(selection, false, selected);
	this._focus();
  	  }else {
alert(lang_s[17]);
}
},
// creating table in the editor
_table : function ()
{
if (this._viewmode == 1){
		var p=prompt(lang_s[15],"");
		var q=prompt(lang_s[16],"");
		if(p!=null&&q!=null&&!isNaN(p)&&!isNaN(q))
			{
			var r='<table style="width: 100%; padding: 0px;border:1px dotted gray;">';
			var t="";
			for(irow=0;irow<p;irow++)
				{
				t+="<tr>";
				for(icol=0;icol<q;icol++)
					{
//					t+='<td style="font-size: 20px; display:table-cell;padding:1px;vertical-align:inherit;border:1px dotted #CCCCCC;">&nbsp;</td>'
					t+='<td style="font-size: 10px; display:table-cell;padding:1px;vertical-align:inherit;border:1px solid #CCCCFF;">&nbsp;</td>'
				}
				t+="</tr>"
			}
			r+=t+"</table><br>";
	this._focus();
    this._HTML('','',r);
	this._focus();
    }
	  	  }else {
alert(lang_s[17]);
}
},
//alternative to insertHTML with all browsers to insert bbcode OR html
_HTML : function (tag,tagend,a)
	{
	this._focus();
	if (this._viewmode == 1){
	var c;
	var u="";
	var d;
	tagend = tagend.replace("=","");
	if(document.all) {
		d=document.getElementById('box').contentWindow;
		d.focus();
		var f=d.document.selection;
		if(f!=null)
			{
			rng=f.createRange();
			u=rng.htmlText;
		}
	} else if (window.getSelection) {
		d=document.getElementById('box').contentWindow;
		d.focus();
		var f=d.getSelection();
		if(f!=""&&f.rangeCount>0)
			{
			rng=f.getRangeAt(f.rangeCount-1).cloneContents();
			var g=d.document.createElement('div');
			g.appendChild(rng);
			u=g.innerHTML;
		}
	}
	if(a){
	var cc = tag+a+tagend;
	}else if(u){
		if(tag=="[code]"||tag=="[code=php]")
		{
				u=u.replace(/[\n\r]/ig,'');
				u=u.replace(/<(br|p|div|li).*?>/ig,"[BR/]");
				u=u.replace(/<\/(p|div).*?>/ig,"");
				u=u.replace(/(<([^>]+)>)/ig,"");
				u=u.replace(/\[BR\/\]/ig,"<br>");
				var cc = tag+u+tagend;	
		}else{
				var cc = tag+u+tagend;
		}
	}else {
		if(tag=="[code]"||tag=="[code=php]"||tag=="[quote]"){
			var cc = tag+tagend;
		}else if(tag =="[video]"){
		var q=prompt(lang_s[14],"");
		if(q!=""&&q!=null){
		var cc = tag+q+tagend;
		}else{
		var cc = '';
		}
		}else{
			var cc = tag+tagend;
			}
	}
	if(isIE)
	{
		d=document.getElementById('box').contentWindow;
		d.document.execCommand("removeformat",false,"");
		d.focus();
		rng.pasteHTML(cc);
		d.focus();
	}
	else if(isWebKit)
		{
		c=document.getElementById('box').contentWindow;
		c.focus();
		u=u.replace(/</g,"[OPEN]");
		u=u.replace(/>/g,"[CLOSE]");
		c.document.execCommand('insertHTML',false,cc);
		var e=document.getElementById('box').contentWindow.document.body.innerHTML;
		u=u.replace(/\[OPEN\]/g,'<');
		u=u.replace(/\[CLOSE\]/g,'>');
		u=u.replace(/[\n\r]/ig,'');
		document.getElementById('box').contentWindow.document.body.innerHTML=e;
		c.focus();
	}
	else
		{
		if(cc!=""&&cc!=null){
		c=document.getElementById('box').contentWindow;
		c.focus();
		c.document.execCommand("removeformat",false,"");
		c.document.execCommand('insertHTML',false,cc);
		c.document.execCommand("removeformat",false,"");
		c.focus();
		}
	}
	  	  }else {
		  if(a){
				bbfontstyle(tag + bbcode._HTMLtoBBcode(a),tagend);
}else{
				bbfontstyle(tag ,tagend);
}
}
},
// insert image in the editor
_image : function () 
{
	if (this._viewmode == 1){
	imagePath = prompt(lang_s[11], 'http://');
	if ((imagePath != null) && (imagePath != "")) {
		var Editor = document.getElementById('box').contentWindow.document;
		this._focus();
		Editor.queryCommandEnabled("InsertImage");
		Editor.execCommand("InsertImage", false, imagePath);
		this._focus();	
		}
		  	  }else {
bbfontstyle('[img]','[/img]');
}
},
_set : function (s) {
	var Editor = document.getElementById('box').contentWindow.document;
	if (!isGecko)
		return;
	try {
		// Try new Gecko method
		Editor.execCommand("styleWithCSS", 0, s);
		} catch (ex) {
		// Use old
		Editor.execCommand("useCSS", 0, !s);
		}
},
//colour EXECCOMMAND
_colour : function (colour,type) 
{
	if (this._viewmode == 1){
	if(isIE && type=="hilitecolor"|| isWebKit && type=="hilitecolor")type="backcolor";
	else type="forecolor";
	var Editor = document.getElementById('box').contentWindow.document;
	this._focus();
	if (isGecko || isOpera && type=="hilitecolor") {
		this._set(true);
	}
	Editor.queryCommandEnabled(type);
	Editor.execCommand(type, false, colour);
	if (isGecko || isOpera && type=="hilitecolor") {
		this._set(false);
	}
	this._focus();
	  	  }else {
		  if(type == "hilitecolor"){
		  	bbfontstyle('[highlight=' + colour +']','[/highlight]');
		  }else{
			bbfontstyle('[color=' + colour +']','[/color]');
		  }
}
},
//toggle BBcode HTML
_toggle : function (colour) 
{
var Editor = document.getElementById('box').contentWindow.document;
if(this._viewmode == 1)
{
	var source = document.getElementById('box_text');
	var editor = document.getElementById('box');
	var switch_b = document.getElementById('switch');
	editor.style.display = 'none';
	source.style.display = 'block';
	switch_b.style.border= 'none';
	switch_b.style.padding= '2px';
	switch_b.style.background= 'none';
	var ceditor = bbcode._HTMLtoBBcode(bbcode._erase(Editor.body.innerHTML));
	source.value = ceditor;
	this._viewmode = 2; // Code
}
else
{
	var source = document.getElementById('box_text');
	var editor = document.getElementById('box');
	var switch_b = document.getElementById('switch');
	editor.style.display = 'block';
	source.style.display = 'none';
	switch_b.style.border= '1px solid #316ac5';
	switch_b.style.padding= '1px';
	switch_b.style.background= '#f1f6f8';
	//writing iframe content and style of the editor
	var iframeContent;
	iframeContent  = '<html xmlns="http://www.w3.org/1999/xhtml" dir="' + lang_s[9] + '" lang="en-gb" xml:lang="en-gb">\n';
	iframeContent += '<head><style type="text/css">body{font-family:Verdana,Arial,Sans-Serif,Tahoma;font-size:12px;background:'+ lang_s[6] +';color:' + lang_s[7] + ';margin:2px;padding:2px;}p {margin:0;padding:0;min-height: 1em; } *+html p { min-height: auto; }div {margin:0;padding:0;}</style></head><body>';	
	if (source.value !== ""){
	var v = source.value;
	iframeContent +=  bbcode._BBcodetoHTML(v) ;
	}
	iframeContent += '</body>';
	iframeContent += '</html>';
	Editor.open();
	Editor.write(iframeContent);
	Editor.close();
		if(isIE){
			Editor.attachEvent('onkeypress', editorEvents);
			Editor.attachEvent('onmousedown', editorEvents);
			Editor.attachEvent('onmousedown', hideall);
			Editor.body.contentEditable = true;
		}else{
			Editor.addEventListener('keypress', editorEvents, true);
			Editor.addEventListener('mousedown', editorEvents, true);
			Editor.addEventListener('mousedown', hideall, true);
			Editor.designMode = "on";
		}
		if(!isWebKit){
		// disable CSS in Geko ,IE and opera  
		try {
			// Try new Gecko method
			Editor.execCommand("styleWithCSS", 0, false);
		} catch (e) {
			// Use old method
			try {Editor.execCommand("useCSS", 0, true);} catch (e) {};
		}
		}
	this._viewmode = 1; // WYSIWYG
}
},
// print colour palette in a table [phpBB3]
_palette : function (type)
{
	var r = 0, g = 0, b = 0;
	var numberlist = new Array(6);
	var color = '';
	numberlist[0] = '00';
	numberlist[1] = '40';
	numberlist[2] = '80';
	numberlist[3] = 'BF';
	numberlist[4] = 'FF';
	document.write(type);
	document.writeln('<table cellspacing="1" cellpadding="0" border="0" onclick="hideall()">');
	for (r = 0; r < 5; r++)
	{
			document.writeln('<tr>');
		for (g = 0; g < 5; g++)
		{
			for (b = 0; b < lang_s[5]; b++)
			{
				color = String(numberlist[r]) + String(numberlist[g]) + String(numberlist[b]);
				document.write('<td bgcolor="#' + color + '" style="width: 15px; height: 10px;">');
				document.write('<a href="#" onClick="comm._colour(\'#' + color + '\',\'' + type +'\'); return false;"><img src="' + lang_s[10] + 'images/editor/spacer.gif" width="15" height="10" alt="#' + color + '" title="#' + color + '" /></a>');
				document.writeln('</td>');
			}
		}
			document.writeln('</tr>');
	}
	document.writeln('</table>');
}

};

//-------  golbal functions -------- //

// gettext into textarea html or bbcode
function Gettext(value,bb) {	
  	 var text = document.getElementById(value);
	 if (comm._viewmode == 2){
	 	var source = document.getElementById('box_text');
	text.value = source.value;
	 comm._viewmode = 1; // WYSIWYG
		 }else {
		 Editor = document.getElementById('box').contentWindow.document;
		 var ceditor = bbcode._HTMLtoBBcode(bbcode._erase(Editor.body.innerHTML));
		 text.value = ceditor;
		 }
  	 
};
// increase and decrease size of the iframe
function textbox_resize(pix)
{
if (comm._viewmode == 1){
	var box			= document.getElementById('box');
	var new_height	= (parseInt(box.style.height) ? parseInt(box.style.height) : 200) + pix;
	if (new_height > 199)
	{
		box.style.height = new_height + 'px';
	}
	return false;
	}else{
		var box			= document.getElementById('box_text');
	var new_height	= (parseInt(box.style.height) ? parseInt(box.style.height) : 200) + pix;
	if (new_height > 110)
	{
		box.style.height = new_height + 'px';
	}
	return false;
	}
};
function hideall(){
 document.getElementById('cph').style.display = 'none';
 document.getElementById('cpf').style.display = 'none';
 document.getElementById('cps').style.display = 'none';
};
//visibility of the color pallette
function toggle_visibility(id,id2) 
{
       var e = document.getElementById(id);
       var buttonElement = document.getElementById(id2);
		e.style.left = getOffsetLeft(buttonElement) + 'px';
		e.style.top = (getOffsetTop(buttonElement) + buttonElement.offsetHeight) + 'px';
       if(e.style.display = ''){
		 hideall();
         e.style.display = 'none';
		 }
       else{
	     hideall();
          e.style.display = '';
		  }
};
// adding quote in topic revision
function addquote(post_id, username){
	var message_name = 'message_' + post_id;
	divarea = document.getElementById(message_name).innerHTML;
	comm._HTML('[quote="' + username + '"]','[/quote]',divarea);
};
// add smiley through path as image
function AddSmileyIcon(imagePath,theme){
	// removing dot from URL
		 if (comm._viewmode == 1){
	imagePath = imagePath.replace(/.\/images\/smilies/gi,"/images/smilies");
		comm._focus();
		comm._HTML(' ',' ',' <img src="'+ imagePath +'" />');
		comm._focus();
		}else{
		bbfontstyle(' ' + theme,' ');
		}
};
//Function to hover button icon
function overIcon(iconItem){
	iconItem.className='oover';
};
//Function to moving off button icon
function outIcon(iconItem){
	iconItem.className='button';
};


/**
* Caret Position object
*/
function caretPosition()
{
	var start = null;
	var end = null;
};


/**
* Get the caret position in an textarea
*/
function getCaretPosition(txtarea)
{
	var caretPos = new caretPosition();
	
	// simple Gecko/Opera way
	if(txtarea.selectionStart || txtarea.selectionStart == 0)
	{
		caretPos.start = txtarea.selectionStart;
		caretPos.end = txtarea.selectionEnd;
	}
	// dirty and slow IE way
	else if(document.selection)
	{
	
		// get current selection
		var range = document.selection.createRange();

		// a new selection of the whole textarea
		var range_all = document.body.createTextRange();
		range_all.moveToElementText(txtarea);
		
		// calculate selection start point by moving beginning of range_all to beginning of range
		var sel_start;
		for (sel_start = 0; range_all.compareEndPoints('StartToStart', range) < 0; sel_start++)
		{		
			range_all.moveStart('character', 1);
		}
	
		txtarea.sel_start = sel_start;
	
		// we ignore the end value for IE, this is already dirty enough and we don't need it
		caretPos.start = txtarea.sel_start;
		caretPos.end = txtarea.sel_start;			
	}

	return caretPos;
};

/**
* Apply bbcodes
*/
function bbfontstyle(bbopen, bbclose)
{
	theSelection = false;

	var textarea = document.getElementById('box_text');

	textarea.focus();

  if (isIE)
   {
      // Get text selection
      theSelection = document.selection.createRange().text;
      var sel = document.selection.createRange();

      if (theSelection)
      {
         // Add tags around selection
         document.selection.createRange().text = bbopen + theSelection + bbclose;
         textarea.focus();
         sel.moveStart('character', bbopen.length);
         sel.moveEnd('character', theSelection.length);
         sel.select();
         theSelection = '';
         return;
      }
   }
	else if (textarea.selectionEnd && (textarea.selectionEnd - textarea.selectionStart > 0))
	{
		mozWrap(textarea, bbopen, bbclose);
		textarea.focus();
		theSelection = '';
		return;
	}
	
	//The new position for the cursor after adding the bbcode
	var caret_pos = getCaretPosition(textarea).start;
	var new_pos = caret_pos + bbopen.length;	
	
	// Open tag
	insert_text(bbopen + bbclose);

	// Center the cursor when we don't have a selection
	// Gecko and proper browsers
	if (!isNaN(textarea.selectionStart))
	{
		textarea.selectionStart = new_pos;
		textarea.selectionEnd = new_pos;
	}	
	// IE
	else if (document.selection)
	{
		var range = textarea.createTextRange(); 
		range.move("character", new_pos); 
		range.select();
		storeCaret(textarea);
	}

	textarea.focus();
	return;
};
function mozWrap(txtarea, open, close)
{
	var selLength = txtarea.textLength;
	var selStart = txtarea.selectionStart;
	var selEnd = txtarea.selectionEnd;
	var scrollTop = txtarea.scrollTop;

	if (selEnd == 1 || selEnd == 2) 
	{
		selEnd = selLength;
	}

	var s1 = (txtarea.value).substring(0,selStart);
	var s2 = (txtarea.value).substring(selStart, selEnd);
	var s3 = (txtarea.value).substring(selEnd, selLength);

	txtarea.value = s1 + open + s2 + close + s3;
    txtarea.selectionStart = selStart + open.length;
    txtarea.selectionEnd = selEnd + open.length;
	txtarea.focus();
	txtarea.scrollTop = scrollTop;

	return;
};
function insert_text(text, spaces, popup)
{
	var textarea;
	
	if (!popup) 
	{
		textarea = document.getElementById('box_text');
	} 
	else 
	{
		textarea = opener.document.getElementById('box_text');
	}
	if (spaces) 
	{
		text = ' ' + text + ' ';
	}
	
	if (!isNaN(textarea.selectionStart))
	{
		var sel_start = textarea.selectionStart;
		var sel_end = textarea.selectionEnd;

		mozWrap(textarea, text, '');
		textarea.selectionStart = sel_start + text.length;
		textarea.selectionEnd = sel_end + text.length;
	}
	else if (textarea.createTextRange && textarea.caretPos)
	{
			textarea.focus();
			storeCaret(textarea);

		var caret_pos = textarea.caretPos;
		caret_pos.text = caret_pos.text.charAt(caret_pos.text.length - 1) == ' ' ? caret_pos.text + text + ' ' : caret_pos.text + text;
	}
	else
	{
		textarea.value = textarea.value + text;
	}
	if (!popup) 
	{
		textarea.focus();
	}
};

function storeCaret(textEl)
{
	if (textEl.createTextRange)
	{
		textEl.caretPos = document.selection.createRange().duplicate();
	}
};
//Iframe top offset
function getOffsetTop(elm){
	var mOffsetTop = elm.offsetTop;
	var mOffsetParent = elm.offsetParent;
	while(mOffsetParent){
		mOffsetTop += mOffsetParent.offsetTop;
		mOffsetParent = mOffsetParent.offsetParent;
	}
	return mOffsetTop;
};

//Iframe left offset
function getOffsetLeft(elm){
	var mOffsetLeft = elm.offsetLeft;
	var mOffsetParent = elm.offsetParent;
	while(mOffsetParent){
		mOffsetLeft += mOffsetParent.offsetLeft;
		mOffsetParent = mOffsetParent.offsetParent;
	}
	return mOffsetLeft;
};
//Run Editor Events
function editorEvents(evt){
	var keyCode = evt.keyCode ? evt.keyCode : evt.charCode;
	var keyCodeChar = String.fromCharCode(keyCode).toLowerCase();
	if(isIE){
		//run if enter key is pressed
	if (evt.type=='keypress' && keyCode==13){
		var editor = document.getElementById('box');
		var selectedRange = editor.contentWindow.document.selection.createRange();
		var parentElement = selectedRange.parentElement();
		var tagName = parentElement.tagName;
		while((/^(a|abbr|acronym|b|bdo|big|cite|code|dfn|em|font|i|kbd|label|q|s|samp|select|small|span|strike|strong|sub|sup|textarea|tt|u|var)$/i.test(tagName)) && (tagName!='HTML')){
			parentElement = parentElement.parentElement;
			tagName = parentElement.tagName;
		}
		//Insert <div> instead of <p>
		if (parentElement.tagName == 'P'||parentElement.tagName=='BODY'||parentElement.tagName=='HTML'||parentElement.tagName=='TD'||parentElement.tagName=='THEAD'||parentElement.tagName=='TFOOT'){
			selectedRange.pasteHTML('<div>');
			selectedRange.select();
			return false;
		}
	}
	hideall();
	return true;
	}else{
  	//Keyboard shortcuts
  	if (evt.type=='keypress' && evt.ctrlKey){
  		var kbShortcut;
  		switch (keyCodeChar){
			case 'b': kbShortcut = 'bold'; break;
			case 'i': kbShortcut = 'italic'; break;
			case 'u': kbShortcut = 'underline'; break;
			case 's': kbShortcut = 'strikethrough'; break;
		}
		if (kbShortcut){
			comm._focus();
			document.getElementById('box').contentWindow.document.queryCommandEnabled(kbShortcut);
			document.getElementById('box').contentWindow.document.execCommand(kbShortcut, false, null);
			comm._focus();
			evt.preventDefault();
			evt.stopPropagation();
		}
	}
	hideall();
	return true;
	}
};