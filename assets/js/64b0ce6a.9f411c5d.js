"use strict";(self.webpackChunkwebserial_docs=self.webpackChunkwebserial_docs||[]).push([[861],{3905:(e,t,r)=>{r.d(t,{Zo:()=>s,kt:()=>m});var a=r(7294);function o(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function n(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);t&&(a=a.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,a)}return r}function i(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?n(Object(r),!0).forEach((function(t){o(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):n(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function l(e,t){if(null==e)return{};var r,a,o=function(e,t){if(null==e)return{};var r,a,o={},n=Object.keys(e);for(a=0;a<n.length;a++)r=n[a],t.indexOf(r)>=0||(o[r]=e[r]);return o}(e,t);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);for(a=0;a<n.length;a++)r=n[a],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(o[r]=e[r])}return o}var p=a.createContext({}),d=function(e){var t=a.useContext(p),r=t;return e&&(r="function"==typeof e?e(t):i(i({},t),e)),r},s=function(e){var t=d(e.components);return a.createElement(p.Provider,{value:t},e.children)},c={inlineCode:"code",wrapper:function(e){var t=e.children;return a.createElement(a.Fragment,{},t)}},u=a.forwardRef((function(e,t){var r=e.components,o=e.mdxType,n=e.originalType,p=e.parentName,s=l(e,["components","mdxType","originalType","parentName"]),u=d(r),m=o,y=u["".concat(p,".").concat(m)]||u[m]||c[m]||n;return r?a.createElement(y,i(i({ref:t},s),{},{components:r})):a.createElement(y,i({ref:t},s))}));function m(e,t){var r=arguments,o=t&&t.mdxType;if("string"==typeof e||o){var n=r.length,i=new Array(n);i[0]=u;var l={};for(var p in t)hasOwnProperty.call(t,p)&&(l[p]=t[p]);l.originalType=e,l.mdxType="string"==typeof e?e:o,i[1]=l;for(var d=2;d<n;d++)i[d]=r[d];return a.createElement.apply(null,i)}return a.createElement.apply(null,r)}u.displayName="MDXCreateElement"},7116:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>p,contentTitle:()=>i,default:()=>c,frontMatter:()=>n,metadata:()=>l,toc:()=>d});var a=r(7462),o=(r(7294),r(3905));const n={title:"Branding",sidebar_label:"Branding (Pro)",sidebar_position:13},i=void 0,l={unversionedId:"branding",id:"branding",title:"Branding",description:"This is an exclusive feature of WebSerial Pro. Check it out here.",source:"@site/docs/branding.md",sourceDirName:".",slug:"/branding",permalink:"/branding",draft:!1,tags:[],version:"current",sidebarPosition:13,frontMatter:{title:"Branding",sidebar_label:"Branding (Pro)",sidebar_position:13},sidebar:"tutorialSidebar",previous:{title:"Custom Title (Pro)",permalink:"/custom-title"},next:{title:"Commercial License",permalink:"/commercial-license"}},p={},d=[{value:"Step 1: Prepare Your Logo",id:"step-1-prepare-your-logo",level:3},{value:"Step 2: Convert Logo Image to a Byte Array",id:"step-2-convert-logo-image-to-a-byte-array",level:3},{value:"Step 3: Modify <code>logo.h</code>",id:"step-3-modify-logoh",level:3},{value:"Step 4: Modify <code>logo.cpp</code>",id:"step-4-modify-logocpp",level:3},{value:"Step 5: Upload the Modified Library",id:"step-5-upload-the-modified-library",level:3},{value:"Wrapping Up",id:"wrapping-up",level:2}],s={toc:d};function c(e){let{components:t,...r}=e;return(0,o.kt)("wrapper",(0,a.Z)({},s,r,{components:t,mdxType:"MDXLayout"}),(0,o.kt)("admonition",{title:"Pro Feature",type:"danger"},(0,o.kt)("p",{parentName:"admonition"},"This is an exclusive feature of WebSerial Pro. Check it out ",(0,o.kt)("a",{parentName:"p",href:"https://webserial.pro"},"here"),".")),(0,o.kt)("br",null),(0,o.kt)("img",{src:"/v2/img/branding.png",alt:"Branding",width:"400px",className:"card-preview"}),(0,o.kt)("br",null),(0,o.kt)("br",null),(0,o.kt)("h1",{id:"overview"},"Overview"),(0,o.kt)("p",null,"Branding in this context refers to the custom logo that is displayed on the WebSerial interface. You can customize the logo by defining the logo image as a byte array in the ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.h")," and ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.cpp")," files of the library's source code."),(0,o.kt)("p",null,"To add your custom logo on the WebSerial interface, follow these steps:"),(0,o.kt)("h3",{id:"step-1-prepare-your-logo"},"Step 1: Prepare Your Logo"),(0,o.kt)("p",null,"First, you need to create or obtain the logo image you want to display on the WebSerial interface. Ensure that the logo image is in a suitable format, such as a SVG (recommended) / PNG or JPG file and make sure your image is optimized & small in file size."),(0,o.kt)("h3",{id:"step-2-convert-logo-image-to-a-byte-array"},"Step 2: Convert Logo Image to a Byte Array"),(0,o.kt)("p",null,"To include the logo in your library, you'll need to convert the image into a byte array."),(0,o.kt)("ol",null,(0,o.kt)("li",{parentName:"ol"},"Go to ",(0,o.kt)("a",{parentName:"li",href:"https://file2raw.labrat.one/"},"File2Raw utility")," ( I created this utility a few years back for the purpose of converting files into byte array for C++ applications )."),(0,o.kt)("li",{parentName:"ol"},"Select your logo."),(0,o.kt)("li",{parentName:"ol"},'Select "Gzip Compress" & "Use PROGMEM (Arduino)".'),(0,o.kt)("li",{parentName:"ol"},'Click "Convert".')),(0,o.kt)("h3",{id:"step-3-modify-logoh"},"Step 3: Modify ",(0,o.kt)("inlineCode",{parentName:"h3"},"logo.h")),(0,o.kt)("ol",null,(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Open the ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.h")," file in your WebSerial library source code.")),(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Locate the ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LIGHT_LOGO")," byte array declaration, which should look like this:"),(0,o.kt)("pre",{parentName:"li"},(0,o.kt)("code",{parentName:"pre",className:"language-cpp"},"extern const uint8_t WEBSERIAL_LIGHT_LOGO[];\n"))),(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Replace the length of the ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LIGHT_LOGO")," array with your custom logo's byte array length.")),(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Locate the ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LIGHT_LOGO_MIME")," variable, which defines the MIME type of your logo image. By default, it is set to ",(0,o.kt)("inlineCode",{parentName:"p"},"image/png"),". If your logo is in a different format (e.g., JPEG), update the ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LIGHT_LOGO_MIME")," variable to the appropriate MIME type.")),(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Set ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LOGO_WIDTH")," & ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LOGO_HEIGHT")," according to your desired size which will be displayed on the webpage."))),(0,o.kt)("h3",{id:"step-4-modify-logocpp"},"Step 4: Modify ",(0,o.kt)("inlineCode",{parentName:"h3"},"logo.cpp")),(0,o.kt)("ol",null,(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Open the ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.cpp")," file in your WebSerial library source code.")),(0,o.kt)("li",{parentName:"ol"},(0,o.kt)("p",{parentName:"li"},"Replace the contents of the ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_LIGHT_LOGO")," array with your custom logo's byte array generated in File2Raw utility."))),(0,o.kt)("h3",{id:"step-5-upload-the-modified-library"},"Step 5: Upload the Modified Library"),(0,o.kt)("p",null,"After making these changes, compile and upload the modified WebSerial library to your device."),(0,o.kt)("h2",{id:"wrapping-up"},"Wrapping Up"),(0,o.kt)("p",null,"Once you have uploaded the modified WebSerial library to your device, That's it! You have successfully added custom branding to the WebSerial interface by defining the logo as a byte array in the ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.h")," and ",(0,o.kt)("inlineCode",{parentName:"p"},"logo.cpp")," files."),(0,o.kt)("p",null,"You can now do the same for ",(0,o.kt)("inlineCode",{parentName:"p"},"WEBSERIAL_DARK_LOGO")," which will be displayed when the UI/interface is in dark mode. ",(0,o.kt)("em",{parentName:"p"},"This is done because some logos need to have inverted colors to appear correctly in dark theme.")),(0,o.kt)("p",null,"Please note that branding, including logos and images, may be subject to copyright and trademark laws. Ensure you have the necessary rights or permissions to use any branding elements in your project."))}c.isMDXComponent=!0}}]);