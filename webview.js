import bindings from 'bindings'

const addon = bindings('webview')

export const createWebview = () => addon.createWebview()
