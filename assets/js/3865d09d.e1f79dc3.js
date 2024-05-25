"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[739],{3905:(e,t,r)=>{r.d(t,{Zo:()=>u,kt:()=>m});var n=r(7294);function a(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function i(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function o(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?i(Object(r),!0).forEach((function(t){a(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):i(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function l(e,t){if(null==e)return{};var r,n,a=function(e,t){if(null==e)return{};var r,n,a={},i=Object.keys(e);for(n=0;n<i.length;n++)r=i[n],t.indexOf(r)>=0||(a[r]=e[r]);return a}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(n=0;n<i.length;n++)r=i[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(a[r]=e[r])}return a}var s=n.createContext({}),c=function(e){var t=n.useContext(s),r=t;return e&&(r="function"==typeof e?e(t):o(o({},t),e)),r},u=function(e){var t=c(e.components);return n.createElement(s.Provider,{value:t},e.children)},p={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},d=n.forwardRef((function(e,t){var r=e.components,a=e.mdxType,i=e.originalType,s=e.parentName,u=l(e,["components","mdxType","originalType","parentName"]),d=c(r),m=a,f=d["".concat(s,".").concat(m)]||d[m]||p[m]||i;return r?n.createElement(f,o(o({ref:t},u),{},{components:r})):n.createElement(f,o({ref:t},u))}));function m(e,t){var r=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var i=r.length,o=new Array(i);o[0]=d;var l={};for(var s in t)hasOwnProperty.call(t,s)&&(l[s]=t[s]);l.originalType=e,l.mdxType="string"==typeof e?e:a,o[1]=l;for(var c=2;c<i;c++)o[c]=r[c];return n.createElement.apply(null,o)}return n.createElement.apply(null,r)}d.displayName="MDXCreateElement"},802:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>s,contentTitle:()=>o,default:()=>p,frontMatter:()=>i,metadata:()=>l,toc:()=>c});var n=r(7462),a=(r(7294),r(3905));const i={title:"Hardware ID",sidebar_label:"Hardware ID (Pro)",sidebar_position:10},o=void 0,l={unversionedId:"hardware-id",id:"hardware-id",title:"Hardware ID",description:"This is an exclusive feature of WebSerial Pro. Check it out here.",source:"@site/docs/hardware-id.md",sourceDirName:".",slug:"/hardware-id",permalink:"/hardware-id",draft:!1,tags:[],version:"current",sidebarPosition:10,frontMatter:{title:"Hardware ID",sidebar_label:"Hardware ID (Pro)",sidebar_position:10},sidebar:"tutorialSidebar",previous:{title:"Printing Logs",permalink:"/printing"},next:{title:"Custom Title (Pro)",permalink:"/custom-title"}},s={},c=[{value:"Introduction",id:"introduction",level:2},{value:"Example Usage:",id:"example-usage",level:3},{value:"Best Practices",id:"best-practices",level:2}],u={toc:c};function p(e){let{components:t,...r}=e;return(0,a.kt)("wrapper",(0,n.Z)({},u,r,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("admonition",{title:"Pro Feature",type:"danger"},(0,a.kt)("p",{parentName:"admonition"},"This is an exclusive feature of WebSerial Pro. Check it out ",(0,a.kt)("a",{parentName:"p",href:"https://webserial.pro"},"here"),".")),(0,a.kt)("br",null),(0,a.kt)("br",null),(0,a.kt)("img",{src:"/v3/img/hardware-id.png",alt:"Hardware ID",width:"400px"}),(0,a.kt)("br",null),(0,a.kt)("h2",{id:"introduction"},"Introduction"),(0,a.kt)("p",null,"The Hardware ID feature is shown on the WebSerial Pro portal for easy identification of multiple devices in your network. This ID should be a unique string that helps you easily identify the specific hardware device among others in your network."),(0,a.kt)("h3",{id:"example-usage"},"Example Usage:"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},'#include <WebSerial.h>\n\nvoid setup() {\n  // Initialize your hardware and other configurations here\n\n  // Set the Hardware ID\n  WebSerial.setID("my_device_001");\n\n  // Additional setup code\n}\n\nvoid loop() {\n  // Your main loop code here\n}\n')),(0,a.kt)("p",null,"In the above example, the ",(0,a.kt)("inlineCode",{parentName:"p"},'WebSerial.setID("my_device_001");'),' function call assigns the Hardware ID "my_device_001" to the device.'),(0,a.kt)("h2",{id:"best-practices"},"Best Practices"),(0,a.kt)("p",null,"To make the most of the Hardware ID feature, consider the following best practices:"),(0,a.kt)("ul",null,(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("p",{parentName:"li"},"Choose a Hardware ID that is unique to each device to avoid conflicts.")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("p",{parentName:"li"},"Avoid using special characters or spaces in the Hardware ID, as these might cause issues when used in network communication or file naming.")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("p",{parentName:"li"},"Document and keep track of the Hardware IDs for all your devices to ensure proper management.")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("p",{parentName:"li"},"Ensure that the Hardware ID is set during the device's initialization phase, such as in the ",(0,a.kt)("inlineCode",{parentName:"p"},"setup()")," function, to guarantee that it is properly configured before any network or communication activities."))))}p.isMDXComponent=!0}}]);