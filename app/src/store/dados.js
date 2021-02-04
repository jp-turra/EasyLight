/* eslint-disable */
const state = {
  page: 'inicio',
  bluetoothState: false,
  device:{}
}

var mutations = {
  setPage (state, payload) {
    if (payload) state.page = payload
    else alert('Erro ao definir página')
  },
  setBluetoothState (state, payload) {
    if (payload !== null && payload !== undefined) state.bluetoothState = payload
    else alert('Erro ao definir bluetoothState')
  },
  setDevice(state, payload){
    if (payload !== null && payload !== undefined) state.device = payload
    else alert('Erro ao definir device')
  }
}

const getters = {
  getPage (state) {
    return state.page
  },
  getBluetoothState (state) {
    return state.bluetoothState
  },
  getDevice (state) {
    return state.device
  }
}

const actions = {
  setAtcion (context, payload) {
    const action = payload[0]
    Array.shift(payload)
    context.commit(action, payload)
  }
}

export default {
  namespaced: false,
  getters,
  mutations,
  actions,
  state
}
