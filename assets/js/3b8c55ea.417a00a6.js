"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[217],{3905:(e,t,r)=>{r.d(t,{Zo:()=>u,kt:()=>d});var a=r(7294);function n(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function i(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);t&&(a=a.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,a)}return r}function l(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?i(Object(r),!0).forEach((function(t){n(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):i(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function o(e,t){if(null==e)return{};var r,a,n=function(e,t){if(null==e)return{};var r,a,n={},i=Object.keys(e);for(a=0;a<i.length;a++)r=i[a],t.indexOf(r)>=0||(n[r]=e[r]);return n}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(a=0;a<i.length;a++)r=i[a],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(n[r]=e[r])}return n}var s=a.createContext({}),p=function(e){var t=a.useContext(s),r=t;return e&&(r="function"==typeof e?e(t):l(l({},t),e)),r},u=function(e){var t=p(e.components);return a.createElement(s.Provider,{value:t},e.children)},c={inlineCode:"code",wrapper:function(e){var t=e.children;return a.createElement(a.Fragment,{},t)}},m=a.forwardRef((function(e,t){var r=e.components,n=e.mdxType,i=e.originalType,s=e.parentName,u=o(e,["components","mdxType","originalType","parentName"]),m=p(r),d=n,h=m["".concat(s,".").concat(d)]||m[d]||c[d]||i;return r?a.createElement(h,l(l({ref:t},u),{},{components:r})):a.createElement(h,l({ref:t},u))}));function d(e,t){var r=arguments,n=t&&t.mdxType;if("string"==typeof e||n){var i=r.length,l=new Array(i);l[0]=m;var o={};for(var s in t)hasOwnProperty.call(t,s)&&(o[s]=t[s]);o.originalType=e,o.mdxType="string"==typeof e?e:n,l[1]=o;for(var p=2;p<i;p++)l[p]=r[p];return a.createElement.apply(null,l)}return a.createElement.apply(null,r)}m.displayName="MDXCreateElement"},9803:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>s,contentTitle:()=>l,default:()=>c,frontMatter:()=>i,metadata:()=>o,toc:()=>p});var a=r(7462),n=(r(7294),r(3905));const i={title:"Installation",sidebar_label:"Installation",sidebar_position:2},l=void 0,o={unversionedId:"installation",id:"installation",title:"Installation",description:"This document explains the installation procedure for getting started with WebSerial.",source:"@site/docs/installation.md",sourceDirName:".",slug:"/installation",permalink:"/installation",draft:!1,tags:[],version:"current",sidebarPosition:2,frontMatter:{title:"Installation",sidebar_label:"Installation",sidebar_position:2},sidebar:"tutorialSidebar",previous:{title:"Introduction",permalink:"/"},next:{title:"Getting Started",permalink:"/getting-started"}},s={},p=[{value:"Dependencies",id:"dependencies",level:3},{value:"For ESP8266",id:"for-esp8266",level:4},{value:"For ESP32",id:"for-esp32",level:4},{value:"Installing WebSerial",id:"installing-webserial",level:3},{value:"1. Directly Through Arduino IDE - Library Manager",id:"1-directly-through-arduino-ide---library-manager",level:4},{value:"2. Manual Install",id:"2-manual-install",level:4},{value:"For Windows",id:"for-windows",level:5},{value:"For Linux",id:"for-linux",level:5},{value:"3. Import through Arduino IDE",id:"3-import-through-arduino-ide",level:4}],u={toc:p};function c(e){let{components:t,...r}=e;return(0,n.kt)("wrapper",(0,a.Z)({},u,r,{components:t,mdxType:"MDXLayout"}),(0,n.kt)("p",null,"This document explains the installation procedure for getting started with WebSerial."),(0,n.kt)("h3",{id:"dependencies"},"Dependencies"),(0,n.kt)("p",null,(0,n.kt)("em",{parentName:"p"},"WebSerial depends on the following libraries present in your libraries folder. Please stricly install the compatible versions of these dependencies only! Using any other version might break WebSerial or may cause WebSerial to work partially.")),(0,n.kt)("h4",{id:"for-esp8266"},"For ESP8266"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/esp8266/Arduino"},"ESP8266 Arduino Core")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/me-no-dev/ESPAsyncTCP"},"ESPAsyncTCP")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/me-no-dev/ESPAsyncWebServer"},"ESPAsyncWebServer")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest"))),(0,n.kt)("h4",{id:"for-esp32"},"For ESP32"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/espressif/arduino-esp32"},"ESP32 Arduino Core")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/me-no-dev/AsyncTCP"},"AsyncTCP")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/me-no-dev/ESPAsyncWebServer"},"ESPAsyncWebServer")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest"))),(0,n.kt)("admonition",{title:"Alternative Dependencies",type:"tip"},(0,n.kt)("p",{parentName:"admonition"},"Although WebSerial supports the ",(0,n.kt)("strong",{parentName:"p"},"official")," dependencies, If you are using WebSerial to send a lot of packets, It's advised to switch to the following ",(0,n.kt)("strong",{parentName:"p"},"fork of dependencies")," for a crash-free experience. These forks are drop-in replacements which contain bugfixes and offer much better performance."),(0,n.kt)("h4",{parentName:"admonition",id:"for-esp8266-1"},"For ESP8266"),(0,n.kt)("ul",{parentName:"admonition"},(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/esphome/ESPAsyncTCP"},"esphome/ESPAsyncTCP")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/mathieucarbou/ESPAsyncWebServer"},"mathieucarbou/ESPAsyncWebServer")," @ ",(0,n.kt)("strong",{parentName:"li"},"2.5.1"))),(0,n.kt)("h4",{parentName:"admonition",id:"for-esp32-1"},"For ESP32"),(0,n.kt)("ul",{parentName:"admonition"},(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/esphome/AsyncTCP"},"esphome/AsyncTCP")," @ ",(0,n.kt)("strong",{parentName:"li"},"latest")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"https://github.com/mathieucarbou/ESPAsyncWebServer"},"mathieucarbou/ESPAsyncWebServer")," @ ",(0,n.kt)("strong",{parentName:"li"},"2.5.1"))),(0,n.kt)("p",{parentName:"admonition"},(0,n.kt)("em",{parentName:"p"},"Please make sure to uninstall the 'original' dependencies from your libraries folder if you have installed any of these forks. If previous dependencies remain installed, the compiler will throw an error."))),(0,n.kt)("h3",{id:"installing-webserial"},"Installing WebSerial"),(0,n.kt)("h4",{id:"1-directly-through-arduino-ide---library-manager"},"1. Directly Through Arduino IDE - Library Manager"),(0,n.kt)("p",null,'Go to Sketch > Include Library > Library Manager > Search for "WebSerial" > Install'),(0,n.kt)("h4",{id:"2-manual-install"},"2. Manual Install"),(0,n.kt)("h5",{id:"for-windows"},"For Windows"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},"Download the ",(0,n.kt)("a",{parentName:"li",href:"https://github.com/ayushsharma82/WebSerial/archive/master.zip"},"Repository")),(0,n.kt)("li",{parentName:"ul"},"Extract the .zip in ",(0,n.kt)("inlineCode",{parentName:"li"},'Documents > Arduino > Libraries > {Place "WebSerial" folder Here}'))),(0,n.kt)("h5",{id:"for-linux"},"For Linux"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},"Download the ",(0,n.kt)("a",{parentName:"li",href:"https://github.com/ayushsharma82/WebSerial/archive/master.zip"},"Repository")),(0,n.kt)("li",{parentName:"ul"},"Extract the .zip in ",(0,n.kt)("inlineCode",{parentName:"li"},'Sketchbook > Libraries > {Place "WebSerial" folder Here}'))),(0,n.kt)("h4",{id:"3-import-through-arduino-ide"},"3. Import through Arduino IDE"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},"Download the ",(0,n.kt)("a",{parentName:"li",href:"https://github.com/ayushsharma82/WebSerial/archive/master.zip"},"Repository")),(0,n.kt)("li",{parentName:"ul"},"Go to ",(0,n.kt)("inlineCode",{parentName:"li"},"Sketch > Include Library > Add .zip Library > Select the Downloaded .zip File."))))}c.isMDXComponent=!0}}]);