<template>
  <q-page class="column content-center q-ma-sm">
    <template v-if="getPage=='inicio'">
      <div class="q-my-sm">
        <q-img
          src='C:\Users\Cliente\Desktop\Oficinas\EasyLight\src\statics\logo.png'
          alt="Logo"
        />
      </div>
      <div class="q-mt-xl q-my-sm">
        <span class="text-h4">Seja bem vindo!</span>
        <q-input class="q-my-sm" v-model="text" type="text" placeholder="Insira seu nome aqui" />
        <q-btn class="full-width" align="center" color="primary" label="ENTRAR" @click="text ? mudarTela('pageOne') : mostrarNotificacao( 'Preencha o seu nome', 'warning') " />
      </div>
    </template>

    <template v-else-if="getPage=='pageOne'">
      <q-list class="full-width q-ma-sm" bordered  v-for="(luz, index) in luzes" :key="index">
        <q-item clickable v-ripple>
          <q-item-section class="col-2" avatar>
            <q-icon color="primary" name="light" />
          </q-item-section>
          <q-item-section class="col-8">
            <span class="text-h6"> {{luz.nome}}<span style="font-size:13px;">{{luz.macBluetooth ? ' - '+luz.macBluetooth : ''}}</span> </span>
            <span class="text-h8 q-mb-sm"> Estado: {{luz.estado ? 'Ligada' : 'Desligada'}}</span>
          </q-item-section>
          <q-item-section avatar class="col-2">
            <q-icon color="secondary" name="delete" />
          </q-item-section>
        </q-item>
        <q-btn class="full-width" color="primary" :label="luz.estado ? 'Desligar' : 'Ligar'" @click="acionarLuz" />
      </q-list>
      <div class="q-mb-xl">
        <q-btn class="full-width" color="green-8" icon="add" label="Adicionar Dispositivo" />
      </div>
    </template>

    <template v-else-if="getPage=='bluetooth'">
      <div class="columns full-width flex-center">
        <div class="q-ma-lg">
          Estado do Bluetooth: <strong>{{getBluetoothState ? 'Ligado' : 'Desligado'}}</strong>
        </div>
        <q-separator class="q-mb-sm" color="black" />
        <div class="q-ma-md" v-if="dispositivos.length!=0">
          <span class="text-bold q-mb-md">Dispositivos disponíveis: </span>
          <q-list bordered v-for="(device, index) in dispositivos" :key="index">
            <q-item clickable v-ripple @click="conectarDispositivo(device)">
              <q-item-section avatar>
                <q-icon color="primary" name="bluetooth_connected" />
              </q-item-section>
              <q-item-section class="columns">
                <span>Nome: {{device.name}}</span>
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
import { Loading } from 'quasar'

export default {
  name: 'PageIndex',
  data () {
    return {
      text: '',
      luzes: [],
      dispositivos: [],
      dispConectado: '',
    }
  },
  mounted () {
    this.getLuzes()
    console.log('page: ', this.getPage)
  },
  computed: {
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
    conectarDispositivo(device){
      Loading.show({message:'Conectando com dispositivo '+device.name})
      let connected = false
      let connect = new Promise( (resolve, reject) => {
        bluetoothClassicSerial.connect(device.id, '00001101-0000-1000-8000-00805f9b34fb', 
          ()=>{
            console.log('OK-CONNECT');
            Loading.hide();
            alert('Dispositivo conectado com sucesso')
            connected = true
            this.$store.commit('setDevice', device)
            this.$store.commit('setPage', 'pageOne')
            resolve('sucesso')
          },
          ()=>{console.log('Deu ruim -connect');Loading.hide();reject('Deu rim')}
        )
      })
      let timeOut = new Promise ((resolve, reject) => {setTimeout(() => {
        Loading.hide()
        if (!connected){
          alert('Não foi possível conectar com o dispositivo '+device.name)
          reject('Deu ruim, time out')
        }      }, 5000);
      })
      
      return Promise.race([connect, timeOut])
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
      let response = 0
      bluetoothClassicSerial.disconnect(
        ()=>{console.log('OK')},
        ()=>{console.log('Deu ruim')}
      )
      console.log(bluetoothClassicSerial)
      Loading.show({message: 'Procurando Dispositivos'})
       await bluetoothClassicSerial.discoverUnpaired(
        (devices) => {
          if (devices.length) console.log(devices)
          else {
            console.log('deu ruim aqui dentro')
            alert('Nenhum dispositivo encontrado.') 
          }
          Loading.hide()
        },
        () => {console.log('deu ruim')}
      )
    },
    getLuzes () {
      let device = this.$store.getters.getDevice
      const aux = { nome: device.name, estado: false, macBluetooth: device.id }
      this.luzes.push(aux)
    },
    mostrarNotificacao (mensagem, tipo) {
      if (!tipo || tipo === 'positive' || tipo === 'negative' || tipo === 'warning') {
        alert(mensagem)
      }
    },
    mudarTela (tela) {
      if (tela) {
        this.$store.commit('setPage', tela)
      }
    },
    acionarLuz () {
      console.log('Acionada')
    }
  }
}
</script>
