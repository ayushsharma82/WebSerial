"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[879],{3905:(e,t,n)=>{n.d(t,{Zo:()=>s,kt:()=>d});var r=n(7294);function a(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function l(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){a(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function o(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},i=Object.keys(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var p=r.createContext({}),u=function(e){var t=r.useContext(p),n=t;return e&&(n="function"==typeof e?e(t):l(l({},t),e)),n},s=function(e){var t=u(e.components);return r.createElement(p.Provider,{value:t},e.children)},c={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},b=r.forwardRef((function(e,t){var n=e.components,a=e.mdxType,i=e.originalType,p=e.parentName,s=o(e,["components","mdxType","originalType","parentName"]),b=u(n),d=a,m=b["".concat(p,".").concat(d)]||b[d]||c[d]||i;return n?r.createElement(m,l(l({ref:t},s),{},{components:n})):r.createElement(m,l({ref:t},s))}));function d(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var i=n.length,l=new Array(i);l[0]=b;var o={};for(var p in t)hasOwnProperty.call(t,p)&&(o[p]=t[p]);o.originalType=e,o.mdxType="string"==typeof e?e:a,l[1]=o;for(var u=2;u<i;u++)l[u]=n[u];return r.createElement.apply(null,l)}return r.createElement.apply(null,n)}b.displayName="MDXCreateElement"},1393:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>p,contentTitle:()=>l,default:()=>c,frontMatter:()=>i,metadata:()=>o,toc:()=>u});var r=n(7462),a=(n(7294),n(3905));const i={title:"Toggle Input",sidebar_label:"Toggle Input (Pro)",sidebar_position:5},l=void 0,o={unversionedId:"toggle-input",id:"toggle-input",title:"Toggle Input",description:"This is an exclusive feature of WebSerial Pro. Check it out here.",source:"@site/docs/toggle-input.md",sourceDirName:".",slug:"/toggle-input",permalink:"/toggle-input",draft:!1,tags:[],version:"current",sidebarPosition:5,frontMatter:{title:"Toggle Input",sidebar_label:"Toggle Input (Pro)",sidebar_position:5},sidebar:"tutorialSidebar",previous:{title:"Printing Logs",permalink:"/printing"},next:{title:"Export Logs (Pro)",permalink:"/export"}},p={},u=[{value:"Disable Input",id:"disable-input",level:2},{value:"Enable Input",id:"enable-input",level:2},{value:"Example Usage",id:"example-usage",level:3}],s={toc:u};function c(e){let{components:t,...n}=e;return(0,a.kt)("wrapper",(0,r.Z)({},s,n,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("admonition",{title:"Pro Feature",type:"danger"},(0,a.kt)("p",{parentName:"admonition"},"This is an exclusive feature of WebSerial Pro. Check it out ",(0,a.kt)("a",{parentName:"p",href:"https://webserial.pro"},"here"),".")),(0,a.kt)("p",null,"The toggle input feature in WebSerial Pro allows users to easily enable or disable the command input bar within the webserial interface. It is particularly useful when users want to use WebSerial solely for monitoring purposes, such as when they do not want commands sent to their firmware or when it is integrated into a product for monitoring only."),(0,a.kt)("p",null,"The input bar can be re-enabled whenever command input is required, making it a versatile tool for various use cases."),(0,a.kt)("h2",{id:"disable-input"},"Disable Input"),(0,a.kt)("br",null),(0,a.kt)("img",{src:"/v2/img/input-bar-disabled.png",alt:"Export Logs Button",width:"1200px"}),(0,a.kt)("br",null),(0,a.kt)("br",null),(0,a.kt)("p",null,"By default, the command input bar is enabled in WebSerial. To disable it, you can simply add this line of code in your firmware:"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},"WebSerial.disableInput();\n")),(0,a.kt)("h2",{id:"enable-input"},"Enable Input"),(0,a.kt)("br",null),(0,a.kt)("img",{src:"/v2/img/input-bar.png",alt:"Export Logs Button",width:"1200px"}),(0,a.kt)("br",null),(0,a.kt)("br",null),(0,a.kt)("p",null,"Similarly, You can re-enable command input bar using the following line of code:"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},"WebSerial.enableInput();\n")),(0,a.kt)("br",null),(0,a.kt)("h3",{id:"example-usage"},"Example Usage"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},"...\n#include <WebSerialPro.h>\n\nvoid setup() {\n  ...\n\n  // Disables WebSerial Command Input Bar\n  WebSerial.disableInput();\n}\n\nvoid loop() {\n  ...\n}\n")))}c.isMDXComponent=!0}}]);