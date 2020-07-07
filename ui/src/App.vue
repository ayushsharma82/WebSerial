<template>
  <div id="app">
    <div class="container mt-2">
      <div class="row justify-content-center my-5">
        <div class="col-auto text-center">
          <img class="logo" draggable="false" :class="{'inactive': !ws.connected}" alt="WebSerial" src="data:image/svg+xml;base64,PHN2ZyBpZD0iTGF5ZXJfMSIgZGF0YS1uYW1lPSJMYXllciAxIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB2aWV3Qm94PSIwIDAgMzAwIDk0Ij48ZGVmcz48c3R5bGU+LmNscy0xe2ZpbGw6dXJsKCNsaW5lYXItZ3JhZGllbnQpO30uY2xzLTJ7ZmlsbDp1cmwoI2xpbmVhci1ncmFkaWVudC0yKTt9LmNscy0ze2ZpbGw6dXJsKCNsaW5lYXItZ3JhZGllbnQtMyk7fS5jbHMtNHtmaWxsOnVybCgjbGluZWFyLWdyYWRpZW50LTQpO30uY2xzLTV7ZmlsbDp1cmwoI2xpbmVhci1ncmFkaWVudC01KTt9LmNscy02e2ZpbGw6dXJsKCNsaW5lYXItZ3JhZGllbnQtNik7fTwvc3R5bGU+PGxpbmVhckdyYWRpZW50IGlkPSJsaW5lYXItZ3JhZGllbnQiIHgxPSItODAuNTEiIHkxPSIyNzMuNTMiIHgyPSItNzkuNTEiIHkyPSIyNzMuNTMiIGdyYWRpZW50VHJhbnNmb3JtPSJtYXRyaXgoMTAuOTMsIDEuNjgsIDUuMSwgLTMuNiwgLTQ3NC4wMiwgMTE1MC44MikiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIj48c3RvcCBvZmZzZXQ9IjAiIHN0b3AtY29sb3I9IiM0MWUyOTYiLz48c3RvcCBvZmZzZXQ9IjEiIHN0b3AtY29sb3I9IiMwMGM0ZWUiLz48L2xpbmVhckdyYWRpZW50PjxsaW5lYXJHcmFkaWVudCBpZD0ibGluZWFyLWdyYWRpZW50LTIiIHgxPSItODMuNTQiIHkxPSIyNzIuMTEiIHgyPSItODIuNTQiIHkyPSIyNzIuMTEiIGdyYWRpZW50VHJhbnNmb3JtPSJtYXRyaXgoMTguNDUsIDEuNjgsIDguNjEsIC0zLjYsIC03NjAuNjYsIDExNTkuNDQpIiB4bGluazpocmVmPSIjbGluZWFyLWdyYWRpZW50Ii8+PGxpbmVhckdyYWRpZW50IGlkPSJsaW5lYXItZ3JhZGllbnQtMyIgeDE9Ii04NS4xNSIgeTE9IjI3MS4zNiIgeDI9Ii04NC4xNSIgeTI9IjI3MS4zNiIgZ3JhZGllbnRUcmFuc2Zvcm09Im1hdHJpeCgyOS4wMywgMS42OCwgMTMuNTMsIC0zLjYsIC0xMTYzLjM3LCAxMTY4LjA2KSIgeGxpbms6aHJlZj0iI2xpbmVhci1ncmFkaWVudCIvPjxsaW5lYXJHcmFkaWVudCBpZD0ibGluZWFyLWdyYWRpZW50LTQiIHgxPSItODMuNTQiIHkxPSIyNzIuMTEiIHgyPSItODIuNTQiIHkyPSIyNzIuMTEiIGdyYWRpZW50VHJhbnNmb3JtPSJtYXRyaXgoMTguNDUsIDEuNjgsIDguNjEsIC0zLjYsIC03NjAuNjYsIDExNzYuNjcpIiB4bGluazpocmVmPSIjbGluZWFyLWdyYWRpZW50Ii8+PGxpbmVhckdyYWRpZW50IGlkPSJsaW5lYXItZ3JhZGllbnQtNSIgeDE9Ii04MC41MSIgeTE9IjI3My41MyIgeDI9Ii03OS41MSIgeTI9IjI3My41MyIgZ3JhZGllbnRUcmFuc2Zvcm09Im1hdHJpeCgxMC45MywgMS42OCwgNS4xLCAtMy42LCAtNDc0LjAyLCAxMTg1LjI5KSIgeGxpbms6aHJlZj0iI2xpbmVhci1ncmFkaWVudCIvPjxsaW5lYXJHcmFkaWVudCBpZD0ibGluZWFyLWdyYWRpZW50LTYiIHgxPSItNzUuNCIgeTE9IjMwMi4wNiIgeDI9Ii03NC40IiB5Mj0iMzAyLjA2IiBncmFkaWVudFRyYW5zZm9ybT0ibWF0cml4KDMuNiwgMTcuOTMsIDEuNjgsIC0zOC40NSwgLTIwMy4zNywgMTMwMDAuNjEpIiB4bGluazpocmVmPSIjbGluZWFyLWdyYWRpZW50Ii8+PC9kZWZzPjx0aXRsZT5BcnRib2FyZCAxPC90aXRsZT48ZyBpZD0iYzMyZDc5MjktYzc1OC05OWQ5LTQ0M2MtMWZmMDY1ZTA1NWVjIj48cGF0aCBkPSJNMTI2LjUyLDM1LjQ4bC02LjEsMjAuMy01LjgtMjAuM2gtMy40N2wtNS44MiwyMC4zLTYuMTEtMjAuM0g5NC44M2w4LjYyLDI2LjEzSDEwN2w1Ljk0LTIwLjEzLDUuOTEsMjAuMTNoMy41bDguNTktMjYuMTNaTTE1MSw1MmMwLTUuNTItMy43Mi05LjM1LTkuMTItOS4zNWE5LjQzLDkuNDMsMCwwLDAtOS41NSw5Ljc0YzAsNS40OSw0LDkuNzIsMTAuMDUsOS43MmExMCwxMCwwLDAsMCw3LjU2LTNsLTIuNzQtMi4yMWE2LjEzLDYuMTMsMCwwLDEtNC44NSwyYy0zLjQ3LDAtNS41Ny0yLjQxLTUuODUtNS43MUgxNTFabS05LjEyLTYuMTZjMi45MSwwLDQuNzgsMS43Myw1LjIsNC42N0gxMzYuNjlDMTM3LjI1LDQ3LjY2LDEzOS4xOCw0NS43OSwxNDEuOSw0NS43OVpNMTY1LDQyLjZhNy43MSw3LjcxLDAsMCwwLTYuMTMsMi42OFYzMy44NmgtMy45NVY2MS42MWgzLjIybC40OC0yLjUyYTcuNzcsNy43NywwLDAsMCw2LjM4LDNjNC44MiwwLDguNjYtMy42Nyw4LjY2LTkuNzJTMTY5Ljg0LDQyLjYsMTY1LDQyLjZabS0xLDE2LjEyYy0zLjQ3LDAtNS43MS0yLjc0LTUuNzEtNi4zOHMyLjI0LTYuNDEsNS43MS02LjQxLDUuNTcsMi44LDUuNTcsNi40MVMxNjcuNDMsNTguNzIsMTY0LjA3LDU4LjcyWm0yMiwzLjQyYzQuMDksMCw4LjkxLTIuMSw4LjkxLTcuNTMsMC00LjE1LTMuMjItNi41NS03LjItNy45M3MtNS4zOC0yLjU0LTUuMzgtNC43LDEuODItMy41Niw0LjE1LTMuNTZhNy4zNCw3LjM0LDAsMCwxLDUuMjksMi4zM0wxOTQuNTEsMzhhMTAuOTEsMTAuOTEsMCwwLDAtOC0zLjE0Yy00LjQ1LDAtOC40MywyLjk0LTguNDMsNy4xNywwLDQuNzksMy4yNSw2LjQ3LDcuNTksOCwyLjY2Ljk1LDQuODQsMi4xOCw0Ljg0LDQuNzNzLTIuNDMsMy44MS00LjczLDMuODFhNy45Miw3LjkyLDAsMCwxLTYuNS0zLjI4bC0yLjk0LDIuODNDMTc4LjY5LDYwLjcxLDE4MS43MSw2Mi4xNCwxODYuMDUsNjIuMTRabTMxLTEwLjE5YzAtNS41Mi0zLjctOS4zNS05LjEtOS4zNWE5LjQzLDkuNDMsMCwwLDAtOS41NSw5Ljc0YzAsNS40OSwzLjk1LDkuNzIsMTAsOS43MmE5LjkxLDkuOTEsMCwwLDAsNy41My0zbC0yLjcxLTIuMjFhNi4yMiw2LjIyLDAsMCwxLTQuODgsMmMtMy40NywwLTUuNTQtMi40MS01LjgyLTUuNzFoMTQuNDhaTTIwOCw0NS43OWMyLjkxLDAsNC43OCwxLjczLDUuMTgsNC42N0gyMDIuNzRDMjAzLjMzLDQ3LjY2LDIwNS4yMyw0NS43OSwyMDgsNDUuNzlabTIxLjY3LTMuMTlhNS4xMyw1LjEzLDAsMCwwLTQuNzMsMi45NGwtLjQ4LTIuNDdIMjIxVjYxLjYxaDRWNTJjMC0zLDEuNi01LjcxLDQuNDgtNS43MWE0LjE4LDQuMTgsMCwwLDEsMS43OS4zNmwxLjIxLTMuMzlBNS45Miw1LjkyLDAsMCwwLDIyOS42NSw0Mi42Wm03LjY0LTMuMDhBMi41MiwyLjUyLDAsMSwwLDIzNC43NywzNywyLjU0LDIuNTQsMCwwLDAsMjM3LjI5LDM5LjUyWm0tMS44NywyMi4wOWgzLjg5VjQzLjA3aC0zLjg5Wk0yNTkuMyw0My4wN2wtLjUzLDIuNThhNy43NSw3Ljc1LDAsMCwwLTYuNDQtM2MtNC44MiwwLTguNjUsMy43Mi04LjY1LDkuNzRzMy44Myw5LjcyLDguNjUsOS43MkE3Ljc3LDcuNzcsMCwwLDAsMjU4Ljc0LDU5bC41NiwyLjYxaDMuMjhWNDMuMDdabS02LjA1LDE1LjY1Yy0zLjMzLDAtNS41NC0yLjc0LTUuNTQtNi4zOHMyLjIxLTYuNDEsNS41NC02LjQxLDUuNzQsMi44LDUuNzQsNi40MVMyNTYuNzIsNTguNzIsMjUzLjI1LDU4LjcyWm0xNC4zOSwyLjg5aDMuOTVWMzMuODZoLTMuOTVaIi8+PC9nPjxnIGlkPSJkZDNiNzA4ZC1mNjVkLTZlNDgtNDk4YS1mNDhjMzA4ZDQzNjgiPjxyZWN0IGNsYXNzPSJjbHMtMSIgeD0iNDEuMzMiIHk9IjI4Ljc4IiB3aWR0aD0iMTIuMDYiIGhlaWdodD0iMy45OCIvPjxyZWN0IGNsYXNzPSJjbHMtMiIgeD0iNDEuMzMiIHk9IjM3LjM5IiB3aWR0aD0iMjAuMzYiIGhlaWdodD0iMy45OCIvPjxyZWN0IGNsYXNzPSJjbHMtMyIgeD0iNDEuMzMiIHk9IjQ2LjAxIiB3aWR0aD0iMzIuMDMiIGhlaWdodD0iMy45OCIvPjxyZWN0IGNsYXNzPSJjbHMtNCIgeD0iNDEuMzMiIHk9IjU0LjYzIiB3aWR0aD0iMjAuMzYiIGhlaWdodD0iMy45OCIvPjxyZWN0IGNsYXNzPSJjbHMtNSIgeD0iNDEuMzMiIHk9IjYzLjI0IiB3aWR0aD0iMTIuMDYiIGhlaWdodD0iMy45OCIvPjxyZWN0IGNsYXNzPSJjbHMtNiIgeD0iMzIuOTMiIHk9IjI2Ljc5IiB3aWR0aD0iMy45OCIgaGVpZ2h0PSI0Mi40MiIvPjwvZz48L3N2Zz4=" width="300px">
        </div>
      </div>
      <div class="row justify-content-center">
        <div class="col-11 col-md-9">
          <div class="card shadow-lg">
            <div class="card-body">
              <div class="container">
                <div class="row justify-content-center">
                  <div class="col-8 col-md-9 col-lg-10">
                      <div class="form-group">
                        <input type="text" class="form-control" placeholder="Type here" v-model="sendInput" v-on:keyup.enter="sendData" :disabled="!ws.connected">
                      </div>
                  </div>
                  <div class="col-4 col-md-3 col-lg-2">
                    <button class="btn btn-primary" @click="sendData" :disabled="!ws.connected">Send</button>
                  </div>
                </div>
              </div>
              <div class="container">
                <div class="row justify-content-center">
                  <div class="col-12">
                    <div class="form-group mt-3">
                      <textarea id="console" class="form-control" rows="8" v-model="serialBuffer" readonly="readonly"></textarea>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Socket from './socket';

export default {
  name: 'webserial',

  data(){
    return{
      ws:{
        connected: false,
        //lastPing: new Date()
      },
      serialBuffer: "",
      sendInput: ""
    }
  },

  methods:{
    sendData(){
      if(this.sendInput.trim() != ""){
        Socket.send(this.sendInput.trim());
        this.sendInput = "";
      }
    }
  },

  mounted(){
    Socket.$on("connected", () => {
      this.ws.connected = true;
    });

    Socket.$on("disconnected", () => {
      this.ws.connected = false;
    });

    Socket.$on("message", (msg) => {
      this.ws.connected = true;
      this.serialBuffer = this.serialBuffer + msg;
      let textarea = this.$el.querySelector("#console");
      textarea.scrollTop = textarea.scrollHeight;
    });
  }
}
</script>

<style lang="scss">
.logo{
  user-select: none;
  -moz-user-select: none;
  -webkit-user-drag: none;
  -webkit-user-select: none;
  -ms-user-select: none;
}

.logo.inactive{
  filter: grayscale(100%);
}

textarea[readonly] {
     cursor: text !important;
}

</style>
