"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[30],{3905:(e,t,r)=>{r.d(t,{Zo:()=>m,kt:()=>d});var n=r(7294);function a(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function o(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function i(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?o(Object(r),!0).forEach((function(t){a(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):o(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function l(e,t){if(null==e)return{};var r,n,a=function(e,t){if(null==e)return{};var r,n,a={},o=Object.keys(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||(a[r]=e[r]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(a[r]=e[r])}return a}var s=n.createContext({}),p=function(e){var t=n.useContext(s),r=t;return e&&(r="function"==typeof e?e(t):i(i({},t),e)),r},m=function(e){var t=p(e.components);return n.createElement(s.Provider,{value:t},e.children)},c={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},u=n.forwardRef((function(e,t){var r=e.components,a=e.mdxType,o=e.originalType,s=e.parentName,m=l(e,["components","mdxType","originalType","parentName"]),u=p(r),d=a,b=u["".concat(s,".").concat(d)]||u[d]||c[d]||o;return r?n.createElement(b,i(i({ref:t},m),{},{components:r})):n.createElement(b,i({ref:t},m))}));function d(e,t){var r=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var o=r.length,i=new Array(o);i[0]=u;var l={};for(var s in t)hasOwnProperty.call(t,s)&&(l[s]=t[s]);l.originalType=e,l.mdxType="string"==typeof e?e:a,i[1]=l;for(var p=2;p<o;p++)i[p]=r[p];return n.createElement.apply(null,i)}return n.createElement.apply(null,r)}u.displayName="MDXCreateElement"},4381:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>s,contentTitle:()=>i,default:()=>c,frontMatter:()=>o,metadata:()=>l,toc:()=>p});var n=r(7462),a=(r(7294),r(3905));const o={title:"Examples",sidebar_label:"Examples",sidebar_position:4},i=void 0,l={unversionedId:"examples",id:"examples",title:"Examples",description:"WebSerial repository comes with Demo and Demo AP examples.",source:"@site/docs/examples.md",sourceDirName:".",slug:"/examples",permalink:"/examples",draft:!1,tags:[],version:"current",sidebarPosition:4,frontMatter:{title:"Examples",sidebar_label:"Examples",sidebar_position:4},sidebar:"tutorialSidebar",previous:{title:"Getting Started",permalink:"/getting-started"},next:{title:"Authentication",permalink:"/authentication"}},s={},p=[{value:"Demo Example",id:"demo-example",level:3},{value:"Demo AP Example",id:"demo-ap-example",level:3}],m={toc:p};function c(e){let{components:t,...r}=e;return(0,a.kt)("wrapper",(0,n.Z)({},m,r,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("p",null,"WebSerial repository comes with Demo and Demo AP examples."),(0,a.kt)("h3",{id:"demo-example"},(0,a.kt)("a",{parentName:"h3",href:"https://github.com/ayushsharma82/WebSerial/blob/master/examples/Demo/Demo.ino"},"Demo Example")),(0,a.kt)("p",null,"Demo example demonstrates a bare minimal sketch with WebSerial integration. Use this as an reference to add WebSerial to your own code."),(0,a.kt)("h3",{id:"demo-ap-example"},(0,a.kt)("a",{parentName:"h3",href:"https://github.com/ayushsharma82/WebSerial/blob/master/examples/Demo_AP/Demo_AP.ino"},"Demo AP Example")),(0,a.kt)("p",null,"Similar to demo, the only difference is that in this example, your microcontroller will be in softAP mode and broadcasting its own SSID (AP). This demonstrates that internet access is not required for WebSerial and it can be used in any environment."),(0,a.kt)("p",null,"You can find those examples in the ",(0,a.kt)("a",{parentName:"p",href:"https://github.com/ayushsharma82/WebSerial/tree/master/examples"},"WebSerial repository"),"."))}c.isMDXComponent=!0}}]);