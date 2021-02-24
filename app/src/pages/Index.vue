<template>
  <q-page class="column content-center q-ma-sm">
    <!-- Login -->
    <template v-if="getPage=='inicio'">
      <div class="q-my-sm">
        <q-img
          src='C:\Users\Cliente\Desktop\Oficinas\EasyLight\src\statics\logo.png'
          alt="Logo"
        />
      </div>
      <div class="q-mt-xl q-my-sm">
        <span class="text-h4">Seja bem vindo!</span>
        <q-input class="q-my-sm" v-model="userName" type="text" placeholder="Insira seu nome aqui" />
        <q-btn class="full-width" align="center" color="primary" label="ENTRAR" @click="login" />
      </div>
    </template>
    <!-- Paginal Inicial -->
    <template v-else-if="getPage=='pageOne'">
      <div v-if="devices.length>0" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Lista de dispositivos disponíveis/pareados -->
      <q-list class="full-width q-ma-sm" bordered  v-for="(luz, index) in devices" :key="index">
        <q-item >
          <q-item-section class="col-2" avatar>
            <q-icon color="primary" name="light" />
          </q-item-section>
          <q-item-section class="col-8">
            <span class="text-h6"> {{luz.name}}<span style="font-size:13px;">{{luz.id ? ' - '+luz.id : ''}}</span> </span>
            <span class="text-h8 q-mb-sm"> Estado: {{luz.estado ? 'Ligada' : 'Desligada'}}</span>
          </q-item-section>
          <q-item-section  avatar class="col-2">
            <q-icon size="8vw" v-ripple color="secondary" name="delete" @click="excluirDipositivo(index)" />
          </q-item-section>
        </q-item>
        <q-btn class="full-width" color="primary" :label="luz.estado ? 'Desligar' : 'Ligar'" @click="enviarDado(luz,'acionar|')" />
      </q-list>
      <div v-if="comandoVoz" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Mostrar comando de voz enviado -->
      <div class="full-width">
        <div v-if="comandoVoz" class="full-width">
          <span  class="text-h8 text-primary">Comando enviado: </span>
        </div>
        <div v-if="comandoVoz" class="full-width text-center">
          <span style="font-size:1.2rem">{{comandoVoz[0]}}<br></span>
        </div>
      </div>
      <div v-if="comandoVoz" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Adicionar Dispositivo -->
      <div class="q-mt-xl" :class="devices.length==0 ? 'fixed-center': ''">
        <q-btn class="full-width" color="green-8" icon="add" label="Adicionar Dispositivo" @click="abrirBluetooth"/>
      </div>
      <!-- Acionar comando de voz -->
      <div class="row fixed-bottom full-width justify-center">
        <q-btn round align="center" class="q-mb-md" color="primary" :loading="listening" icon="mic" size="20px" @click="listening ? StopSpeechRecognition : StartSpeechRecognition" />
      </div>
    </template>
    <!-- Configuração de dispositivo / Bluetooh -->
    <template v-else-if="getPage=='bluetooth'">
      <div class="columns full-width flex-center">
        <div class="q-ma-lg">
          Estado do Bluetooth: <strong>{{getBluetoothState ? 'Ligado' : 'Desligado'}}</strong>
        </div>
        <q-separator class="q-mb-sm" color="black" />
        <div class="q-ma-md" v-if="dispositivos.length!=0">
          <span class="text-bold q-mb-md">Dispositivos disponíveis: </span>
          <q-list bordered v-for="(device, index) in dispositivos" :key="index">
            <q-item v-ripple >
              <q-item-section avatar>
                <q-btn class="full-width" color="primary" icon="bluetooth_connected"  @click="conectarDispositivo(device)"/>
              </q-item-section>
              <q-item-section class="columns">
                <q-input v-model="device.name" type="text" />
                <span>Mac: {{device.id}}</span>
              </q-item-section>
            </q-item>
          </q-list>
        </div>
        <div v-else-if="dispositivos.length==0 && getBluetoothState">
          <q-btn class="full-width q-ma-md" color="primary" icon="search" label="Procurar dispositivo" @click="procurarDispositivos" />
          <q-btn class="full-width q-ma-md" color="primary" label="Carregar lista de dispositivos pareados" @click="dispositivosPareados" />
        </div>
      </div>
    </template>
  </q-page>
</template>

<script>
/* eslint-disable */
import {  } from 'cordova-plugin-bluetoothclassic-serial/www/bluetoothClassicSerial.js'
import { Loading, Notify, Dialog } from 'quasar'

const option = {
  language:"pt-BR",
  matches:5,
  prompt:"",      // Android only
  showPopup:false,  // Android only
  showPartial:false 
}

export default {
  name: 'PageIndex',
  data () {
    return {
      text: '',
      luzes: [],
      dispositivos: [],
      dispConectado: '',
      comandoVoz:'',
      listening:false,
      userName:'',
    }
  },
  created() {
    this.userName = this.$store.getters.getUserName
    if (this.userName) this.mudarTela('pageOne')
  },
  mounted () {
    this.getLuzes()
    console.log('page: ', this.getPage)
  },
  computed: {
    devices () {
      return this.$store.getters.getDevice
    },
    logo () {
      return ''
    },
    getPage () {
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      return this.$store.getters.getPage || 'inicio'
    },
    getBluetoothState() {
      return this.$store.getters.getBluetoothState || false
    }
  },
  methods: {
    // ****************** LOGIN *************************
    login(){
      if(this.userName){
        this.mudarTela('pageOne') 
        this.$store.commit('setUserName', this.userName)
      }else{
        this.mudarTela('pageOne') 
        this.$store.commit('setUserName', "Anônimo")
      }
    },

    // ****************** SPEECH RECOGNITION *************************
    StartSpeechRecognition(){
      console.log('Recording')
      this.listening = true
      window.plugins.speechRecognition.startListening(
        (success)=>{
          this.comandoVoz = success
          this.listening = false
          this.enviarDado({}, this.comandoVoz[0].toLowerCase()+"|")
        },
        ()=>{console.log("Error on recognizing")},
        option
      )
    },
    StopSpeechRecognition(){
      console.log('Stop recording')
      window.plugins.speechRecognition.stopListening(
        (success)=>{console.log("Recognition stopped"); this.listening=false},
        ()=>{console.log("Error on recognize")},
      )
    },
    // ****************** BLEUTOOTH *************************
    abrirBluetooth () {
      bluetoothClassicSerial.isEnabled(
        () => { this.$store.commit('setBluetoothState', true) },
        () => {
          if (this.$q.platform.is.android) {
            bluetoothClassicSerial.enable(
              () => { this.$store.commit('setBluetoothState', true) },
              () => { this.$store.commit('setBluetoothState', false); this.$store.commit('setPage', 'pageOne') }
            )
          } else {
            alert('Ative o Bluetooth')
            this.$store.commit('setPage', 'pageOne')
          }
        }
      )
      this.$store.commit('setPage', 'bluetooth')
    },
    dispositivosPareados(){
      bluetoothClassicSerial.list(
        (devices)=>{
          if (devices.length) {  
            devices.forEach((device)=>{
              this.dispositivos.push(device)
            })
            console.log(this.dispositivos)
          }
        },
        ()=>{console.log('sem nada')}
      )
    },
    async procurarDispositivos(){
      console.log(bluetoothClassicSerial)
      let response = 0
      Loading.show({message: 'Procurando Dispositivos'})
      let promise = new Promise((resolve, reject)=>{
        bluetoothClassicSerial.discoverUnpaired(
          (devices) => {
            console.log('Sem filtro', devices)
            if (devices) {console.log(devices);resolve(devices)}
            else {
              //Notify.create({message:'Nenhum dispositivo encontrado.'}) 
              Loading.hide()
              reject(null)
            }
          },
          () => {console.log('deu ruim');Loading.hide();}
        )
      })
      let timeoutPromise = new Promise((resolve, reject)=>{setTimeout(() => {
        reject(null)
      }, 10000);})
      
      console.log('Start promisse')
      response = await Promise.race([promise, timeoutPromise])
      if (response) console.log('response: ', response)
      else console.log("Nulo")
    },
    async enviarDado(device,dado){
      console.log(device)
      dado = new String(dado).toString()
      if (dado){
        await bluetoothClassicSerial.write("00001101-0000-1000-8000-00805F9B34FB", dado, 
          ()=>{console.log('dado a enviado: ', dado)}, 
          ()=>{console.log('n enviou')}
        );
      }
    },
    conectarDispositivo(device){
      Loading.show({message:'Conectando com dispositivo '+ device.name})
      let connected = false
      let connect = new Promise( (resolve, reject) => {
        bluetoothClassicSerial.connect(device.id, "00001101-0000-1000-8000-00805F9B34FB", 
          ()=>{
            console.log('OK-CONNECT');
            Loading.hide();
            connected = true
            this.$store.commit('addDevice', device)
            this.$store.commit('setPage', 'pageOne')
            resolve('sucesso')
          },
          ()=>{console.log('Deu ruim -connect');Loading.hide();reject(null)}
        )
      })
      let timeOut = new Promise ((resolve, reject) => {setTimeout(() => {
        Loading.hide()
        if (!connected){
          reject(null)
        }}, 5000);
      })
      
      return Promise.race([connect, timeOut])
    },
    

    // ****************** FUNCÕES GERAIS *************************
    getLuzes () {
      console.log('Device: ', this.devices)
      if (this.devices){
        for (device of this.devices) {
          const aux = { nome: this.devices.name, estado: false, macBluetooth: this.devices.id }
          this.luzes.push(aux)
        }
      }
    },
    excluirDipositivo(index) {
      let devices = this.devices
      devices = this.drop(devices, index)
      this.$store.commit('setDevice', devices)
    },
    mudarTela (tela) {
      if (tela) {
        this.$store.commit('setPage', tela)
      }
    },
    drop (array, position) {
      let aux = []
      array.forEach((val,i) => {
        if (i != position){
          aux.push(val)
        }
      })
      return aux
    }
  }
}
</script>
