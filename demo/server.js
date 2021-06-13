import express from 'express'
import { createWebview } from '../webview.js'
import { dirname } from 'path'
import { fileURLToPath } from 'url'

const __dirname = dirname(fileURLToPath(import.meta.url))

const app = express()

app.use(express.static(__dirname))

setInterval(() => {
  console.log('hello from server')
}, 100)

app.listen(3000, () => {
  const webview = createWebview()
})
