<template>
  <div id="q-app">
    <router-view />
  </div>
</template>
<script>
export default {
  name: 'App',
  created () {
    window.plugins.speechRecognition.isRecognitionAvailable(
      (success) => {
        console.log('Recognition Available')
        window.plugins.speechRecognition.hasPermission(
          (success) => { console.log('Has permission'); this.$store.commit('setRecognitionStatus', true) },
          () => {
            console.log("Hasn't permission")
            window.plugins.speechRecognition.requestPermission(
              (success) => { console.log('Recognition Authorized'); this.$store.commit('setRecognitionStatus', true) },
              () => { console.log('Recognition Unauthorized'); this.$store.commit('setRecognitionStatus', false) }
            )
          }
        )
      },
      () => { console.log('Recognition Unavailable'); this.$store.commit('setRecognitionStatus', false) }
    )
    document.addEventListener('deviceready', () => { window.open = cordova.InAppBrowser.open }, false)
  }
}
</script>
