"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[868],{3905:(e,t,r)=>{r.d(t,{Zo:()=>c,kt:()=>f});var n=r(7294);function a(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function o(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function i(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?o(Object(r),!0).forEach((function(t){a(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):o(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function s(e,t){if(null==e)return{};var r,n,a=function(e,t){if(null==e)return{};var r,n,a={},o=Object.keys(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||(a[r]=e[r]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(a[r]=e[r])}return a}var l=n.createContext({}),p=function(e){var t=n.useContext(l),r=t;return e&&(r="function"==typeof e?e(t):i(i({},t),e)),r},c=function(e){var t=p(e.components);return n.createElement(l.Provider,{value:t},e.children)},m={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},u=n.forwardRef((function(e,t){var r=e.components,a=e.mdxType,o=e.originalType,l=e.parentName,c=s(e,["components","mdxType","originalType","parentName"]),u=p(r),f=a,d=u["".concat(l,".").concat(f)]||u[f]||m[f]||o;return r?n.createElement(d,i(i({ref:t},c),{},{components:r})):n.createElement(d,i({ref:t},c))}));function f(e,t){var r=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var o=r.length,i=new Array(o);i[0]=u;var s={};for(var l in t)hasOwnProperty.call(t,l)&&(s[l]=t[l]);s.originalType=e,s.mdxType="string"==typeof e?e:a,i[1]=s;for(var p=2;p<o;p++)i[p]=r[p];return n.createElement.apply(null,i)}return n.createElement.apply(null,r)}u.displayName="MDXCreateElement"},8256:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>l,contentTitle:()=>i,default:()=>m,frontMatter:()=>o,metadata:()=>s,toc:()=>p});var n=r(7462),a=(r(7294),r(3905));const o={title:"Timestamps",sidebar_label:"Timestamps (Pro)",sidebar_position:7},i=void 0,s={unversionedId:"timestamps",id:"timestamps",title:"Timestamps",description:"This is an exclusive feature of WebSerial Pro. Check it out here.",source:"@site/docs/timestamps.md",sourceDirName:".",slug:"/timestamps",permalink:"/timestamps",draft:!1,tags:[],version:"current",sidebarPosition:7,frontMatter:{title:"Timestamps",sidebar_label:"Timestamps (Pro)",sidebar_position:7},sidebar:"tutorialSidebar",previous:{title:"Export Logs (Pro)",permalink:"/export"},next:{title:"Font Settings (Pro)",permalink:"/font"}},l={},p=[],c={toc:p};function m(e){let{components:t,...r}=e;return(0,a.kt)("wrapper",(0,n.Z)({},c,r,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("admonition",{title:"Pro Feature",type:"danger"},(0,a.kt)("p",{parentName:"admonition"},"This is an exclusive feature of WebSerial Pro. Check it out ",(0,a.kt)("a",{parentName:"p",href:"https://webserial.pro"},"here"),".")),(0,a.kt)("br",null),(0,a.kt)("img",{src:"/v2/img/timestamps.png",alt:"Export Logs Button",width:"500px",className:"card-preview"}),(0,a.kt)("br",null),(0,a.kt)("br",null),(0,a.kt)("p",null,"Timestamps lets you log messages with time added to start of each line. It's a great way to keep track of your logs and when a certain event happens. Ultimately it's can extremely useful tool in most scenarios which can used to analyze scheduled tasks, timers, and task execution times."),(0,a.kt)("p",null,"The timestamps feature has been re-worked on with the v2 release of WebSerial Pro. Timestamps in WebSerial Pro are now syncronized with your microcontroller without any external dependency using a clever trick! This keeps the time same across multiple opened windows and it's unaffected by the latency of transmission."),(0,a.kt)("br",null),(0,a.kt)("img",{src:"/v2/img/timestamps-settings.png",alt:"Export Logs Button",width:"1200px",className:"card-preview"}),(0,a.kt)("br",null),(0,a.kt)("br",null),(0,a.kt)("p",null,'Users can easily enable the timestamps via their WebSerial settings panel. To further improve the user experience, if you don\'t want the timestamps to be selectable while you try to copy text from the terminal, then you can also easy switch that off using "Prevent Copying" toggle button.'))}m.isMDXComponent=!0}}]);