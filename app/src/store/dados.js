
const state = {
  page: 'inicio',
  bluetoothState: false,
  device: [],
  userName: null,
  recognitionStatus: false
}

var mutations = {
  setRecognitionStatus (state, status) {
    state.recognitionStatus = status
  },
  setUserName (state, name) {
    state.userName = name
  },
  setPage (state, payload) {
    state.page = payload
  },
  setBluetoothState (state, payload) {
    state.bluetoothState = payload
  },
  setDevice (state, payload) {
    state.device = payload
  },
  addDevice (state, device) {
    console.log(device)
    if (device) state.device.push(device)
  }
}

const getters = {
  getRecognitionStatus (state) {
    return state.recognitionStatus
  },
  getUserName (state) {
    return state.userName
  },
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
  logOut (context) {
    context.commit('setUserName', null)
    context.commit('setDevice', null)
    context.commit('setPage', 'inicio')
  },
  updateDevice (context, payload) {
    const device = payload
    // eslint-disable-next-line prefer-const
    let devices = context.getters.getDevice
    const index = devices.indexOf(device)
    devices.forEach((val, i) => {
      if (index === i) {
        devices[index] = device
      }
    })
    context.commit('setDevice', devices)
  }
}

export default {
  namespaced: false,
  getters,
  mutations,
  actions,
  state
}
