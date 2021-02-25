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
      <q-list class="full-width q-ma-sm" bordered  v-for="(device, index) in devices" :key="index">
        <q-item >
          <q-item-section class="col-2" avatar>
            <q-icon color="primary" name="light" />
          </q-item-section>
          <q-item-section class="col-8">
            <span class="text-h6"> {{device.name}}<span style="font-size:13px;">{{device.id ? ' - '+device.id : ''}}</span> </span>
            <div v-if="device.comodos" class="full-width row">
              <div class="col-12" v-for="(comodo, index) in device.comodos" :key="index">
                <span>Comodo {{index}} - {{comodo.name}}</span>
              </div>
            </div>
          </q-item-section>
          <q-item-section  avatar class="col-2">
            <q-icon size="8vw" v-ripple color="secondary" name="settings" @click="editarComodos=device.comodos||[];editarEquipamento=device;editarEquipamentoModal=true" />
          </q-item-section>
        </q-item>
        <q-btn class="full-width" color="primary" :label="dispConectado ? 'Acionar' : 'Conectar'" @click="dispConectado ? enviarDado(device,'acionar') : conectarDispositivo(device)" />
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
      <div v-if="devices.length>0" class="row fixed-bottom full-width justify-center">
        <q-btn round :class="listening ? 'pulse-button' : ''" align="center" class="q-mb-md" color="primary" icon="mic" :size="listening?'8vw':'6vw'" @click="listening? StopSpeechRecognition():StartSpeechRecognition()" />
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
                <q-btn class="full-width" color="primary" icon="bluetooth_connected"  @click="conectarDispositivo(device,  true)"/>
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
    <!-- Definições de equipamento -->
    <q-dialog v-model="editarEquipamentoModal" maximized>
      <q-card>
        <q-card-section class="columns items-center">
          <span class="text-h6 text-bold">Configuração de equipamento</span><br>
          <span class="text-h8">Id: {{editarEquipamento.id}}</span>
        </q-card-section>
        <q-card-section class="full-width">
          <q-separator color="secondary" />
        </q-card-section>
        <q-card-section>
          <q-input class="text-h6" color="primary" v-model="editarEquipamento.name" type="text" label="Nome do Equipamento">
            <q-btn flat color="primary" icon="clear" size="4vw" @click="editarEquipamento.name=''" />
          </q-input>
        </q-card-section>
        <q-card-section>
          <div class="ful-width" v-for="(comodo, index) in editarComodos" :key="index">
            <q-input class="text-h6" color="primary" v-model="comodo.name" type="text" label="Nome do Comodo">
              <q-btn flat color="primary" icon="clear" size="4vw" @click="comodo.name=''" />
            </q-input>
          </div>
        </q-card-section>
        <q-card-section v-if="!editarComodos || editarComodos.length < 3">
          <q-btn class="full-width" color="primary" icon="add" label="Adicionar comodo" @click="editarComodos.push({name:''})" />
        </q-card-section>
        <q-card-section class="q-mt-xl">
          <q-btn class="full-width" color="red-10" icon="delete" label="Deletar Dispositivo" @click="excluirDipositivo(editarEquipamento)" />
        </q-card-section>
        <q-card-actions align="around" class="" style="bottom:0px">
          <q-separator class="full-width q-mb-sm" color="secondary" />
          <q-btn style="width:40%" label="Cancelar" color="red-8" v-close-popup />
          <q-btn style="width:40%" label="Salvar" color="green-8" v-close-popup @click="salvarAlteracoesEquipamento(editarEquipamento, editarComodos)"/>
        </q-card-actions>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
/* eslint-disable */
import {  } from 'cordova-plugin-bluetoothclassic-serial/www/bluetoothClassicSerial.js'
import { Dialog, Loading } from 'quasar'

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
      editarEquipamentoModal:false,
      editarEquipamento:{},
      editarComodos:[]
    }
  },
  created() {
    this.userName = this.$store.getters.getUserName
    if (this.userName) this.mudarTela('pageOne')
  },
  mounted () {
    this.getLuzes()
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
          this.acionarDispositivo(success[0].toLowerCase())
        },
        ()=>{console.log("Error on recognizing"); this.listening=false},
        option
      )
    },
    StopSpeechRecognition(){
      console.log('Stop recording')
      window.plugins.speechRecognition.stopListening(
        (success)=>{console.log("Recognition stopped");},
        ()=>{console.log("Error on stop recognition");},
      )
      this.listening=false
    },
    async acionarDispositivo(comando){
      comando = comando.split(" ")
      let action = comando.length>1 ? comando[0] : comando
      let dispName = comando.length>=2 ? comando.length>2 ? comando.slice(1).toString().replaceAll(","," ") : comando[1] : null
      let device
      if (dispName && dispName!="") device = this.getDeviceByName(dispName)
      console.log(device)
      if (!device) {alert("Nenhum dispositivo encontrado");return}
      else this.enviarDado(device, action)
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
      let response
      if (this.dispConectado&&device&&this.dispConectado.id==device.id) response="sucesso"
      else{ response = await this.conectarDispositivo(device, false) }
      Loading.show({message:"Enviando comando "+dado})
      let timeout = this.dispConectado ? 0 : 5000
      setTimeout(() => {
        console.log("response: ", response)
        if (response="sucesso" && this.dispConectado) {
          try{dado = new String(dado).toString()}
          catch{return}
          if (dado){
            bluetoothClassicSerial.write("00001101-0000-1000-8000-00805F9B34FB", dado+"|", 
              ()=>{
                console.log('dado a enviado: ', dado)
                Loading.hide()
              }, 
              ()=>{console.log('n enviou');Loading.hide()}
            );
          }  ;
        }
      }, timeout);
    },
    async conectarDispositivo(device, cadastro){
      Loading.show({message:'Conectando com dispositivo '+ device.name})
      bluetoothClassicSerial.connect(device.id, "00001101-0000-1000-8000-00805F9B34FB", 
        ()=>{
          console.log('OK-CONNECT');
          let isDeviceSet = cadastro ? this.hasDevice(device) : true
          this.dispConectado = device
          if(cadastro || isDeviceSet)Loading.hide();
          if (cadastro && !isDeviceSet) {
            this.$store.commit('addDevice', device)
            this.$store.commit('setPage', 'pageOne')
          }
          return 'sucesso'
        },
        ()=>{console.log('Device ', device.id, ' disconnected');this.dispConectado=null;}
      )
      setTimeout(() => {
        Loading.hide()
        if (!this.dispConectado) {this.desconectarTodos();alert("Falha ao conectar com dispositivo, tente novamente.");}
      }, 10000);
    },
    desconectarTodos(){
      bluetoothClassicSerial.disconnect(()=>{console.log("Desconectado com sucesso")}, ()=>{console.log("Erro ao desconectar")});
    },

    // ****************** FUNCÕES GERAIS *************************
    getLuzes () {
      if (this.devices){
        for (let device of this.devices) {
          const aux = { nome: device.name, estado: false, macBluetooth: device.id }
          this.luzes.push(aux)
        }
      }
    },
    getDeviceByName(info){
      let match = null;
      this.devices.forEach((device, index)=>{
        for (let prop in Object(device)){
          if (device[prop].toString().toLowerCase() == info.toString().toLowerCase()){
            match = device
          }
        }
      })
      return match
    },
    hasDevice(device){
      let have = false
      this.devices.forEach((val, index) => {
        if (val.id == device.id) {
          alert("Dispositivo já adicionado")
          have = true
        }
      })
      return have
    },
    salvarAlteracoesEquipamento(device, comodos){
      device.comodos = []
      comodos.forEach((val)=>{
        if (val.name || val.name!="") device.comodos.push(val)
      })

      console.log("Equipamento salvo: ", device)
      this.$store.dispatch('updateDevice', device)
    },
    excluirDipositivo(device) {
      Dialog.create({
        title:'Alerta',
        message:'Você tem certeza que deseja excluir o equipamento?',
        ok:'Sim',
        cancel:'Cancelar'
      }).onOk(()=>{
        let devices = this.devices
        let index = devices.indexOf(device)
        devices = this.drop(devices, index)
        this.$store.commit('setDevice', devices)
        this.editarEquipamentoModal = false
      })
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

<style scoped>
.pulse-button {
  margin-bottom: 15vw;
  border: none;
  box-shadow: 0 0 0 0 #26A69A;
  border-radius: 50%;
  background-size:cover;
  cursor: pointer;
  -webkit-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  -moz-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  -ms-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
}

@-webkit-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@-moz-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@-ms-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
</style>