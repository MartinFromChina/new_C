(function(){/* 
 
 Copyright The Closure Library Authors. 
 SPDX-License-Identifier: Apache-2.0 
*/ 
'use strict';(class{constructor(a,b){this.g=b===f?a:""}}).prototype.toString=function(){return this.g.toString()};var f={};function g(a,b,d){a.addEventListener&&a.addEventListener(b,d,!1)};function h(a,b,d){if(Array.isArray(b))for(var c=0;c<b.length;c++)h(a,String(b[c]),d);else null!=b&&d.push(a+(""===b?"":"="+encodeURIComponent(String(b))))};function k(a,b,d=null){l(a,b,d)}function l(a,b,d){a.google_image_requests||(a.google_image_requests=[]);const c=a.document.createElement("img");if(d){const e=C=>{d&&d(C);c.removeEventListener&&c.removeEventListener("load",e,!1);c.removeEventListener&&c.removeEventListener("error",e,!1)};g(c,"load",e);g(c,"error",e)}c.src=b;a.google_image_requests.push(c)};function m(a=null){return a&&"22"===a.getAttribute("data-jc")?a:document.querySelector('[data-jc="22"]')};var n=document,p=window;var q={};function r(){if(q!==q)throw Error("Bad secret");};function t(){var a="undefined"!==typeof window?window.trustedTypes:void 0;return null!==a&&void 0!==a?a:null};var u,v=class{};class w extends v{constructor(){var a=null!==x&&void 0!==x?x:"";super();r();this.g=a}toString(){return this.g.toString()}}var x=null===(u=t())||void 0===u?void 0:u.emptyHTML;new w;var y,z=class{};class A extends z{constructor(){var a=null!==B&&void 0!==B?B:"";super();r();this.g=a}toString(){return this.g.toString()}}var B=null===(y=t())||void 0===y?void 0:y.emptyScript;new A;var D=class{};class E extends D{constructor(a){super();r();this.g=a}toString(){return this.g}}new E("about:blank");new E("about:invalid#zTSz");function F(a){return"undefined"!==typeof a}function G(a){g(n,a.i,()=>{if(n[a.h])a.g&&(a.g=!1,a.j=Date.now(),H(a,0));else{if(-1!==a.j){const b=Date.now()-a.j;0<b&&(a.j=-1,H(a,1,b))}H(a,3)}})}function I(a){g(p,"click",b=>{a.handleClick(b)})} 
function H(a,b,d=0){var c={gqid:a.m,qqid:a.o};0===b&&(c["return"]=0);1===b&&(c["return"]=1,c.timeDelta=d);2===b&&(c.bgload=1);3===b&&(c.fg=1);b=[];for(var e in c)h(e,c[e],b);k(p,a.l+"&label=window_focus&"+b.join("&"),void 0);if(!(.01<Math.random())){a=(a=m(document.currentScript))&&a.getAttribute("data-jc-version")||"unknown";a=`https://${"pagead2.googlesyndication.com"}/pagead/gen_204?id=jca&jc=${22}&version=${a}&sample=${.01}`;c=window;if(e=c.navigator)e=c.navigator.userAgent,e=/Chrome/.test(e)&& 
!/Edge/.test(e)?!0:!1;e&&c.navigator.sendBeacon?c.navigator.sendBeacon(a):k(c,a)}} 
var K=class{constructor(){var a=J["gws-id"],b=J["qem-id"];this.l=J.url;this.m=a;this.o=b;this.g=!1;a=F(n.hidden)?{h:"hidden",i:"visibilitychange"}:F(n.mozHidden)?{h:"mozHidden",i:"mozvisibilitychange"}:F(n.msHidden)?{h:"msHidden",i:"msvisibilitychange"}:F(n.webkitHidden)?{h:"webkitHidden",i:"webkitvisibilitychange"}:{h:"hidden",i:"visibilitychange"};this.h=a.h;this.i=a.i;this.j=-1;n[this.h]&&H(this,2);G(this);I(this)}handleClick(){this.g=!0;p.setTimeout(()=>{this.g=!1},5E3)}};{const a=m(document.currentScript);if(null==a)throw Error("JSC not found 22");var J;{const b={},d=a.attributes;for(let c=d.length-1;0<=c;c--){const e=d[c].name;0===e.indexOf("data-jcp-")&&(b[e.substring(9)]=d[c].value)}J=b}window.window_focus_for_click=new K};}).call(this);
