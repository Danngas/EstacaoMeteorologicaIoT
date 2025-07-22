#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// Macros para conversão de variáveis em strings
#define TOSTRING(x) #x
#define STRINGIFY(x) TOSTRING(x)

// Buffers das variáveis (devem ser atualizados no main.c)
extern char str_tmp1[5];  // Temp. BMP280 (ex.: "23.5")
extern char str_tmp2[5];  // Temp. AHT20 (ex.: "24.7")
extern char str_umi[5];   // Umidade (ex.: "55.2")
extern char str_alt[5];   // Altitude (ex.: "500" m)

// Offsets para calibração (devem ser persistidos no main.c)
extern float offset_bmp;  // Offset para BMP280
extern float offset_aht;  // Offset para AHT20
extern float offset_umi;  // Offset para Umidade
extern float offset_alt;  // Offset para Altitude

// Variáveis de alerta
extern uint16_t alerta_temp_min;  // Temperatura limite mínima
extern uint16_t alerta_temp_max;  // Temperatura limite máxima
extern uint16_t alerta_umi_minima; // Umidade limite mínima
extern uint16_t alerta_umi_maxima; // Umidade limite máxima

// HTML principal com seções Gráficos, Sensores, Calibração e Alertas (~1300 bytes)
const char HTML_BODY[] =
"<!DOCTYPE html><html lang='pt-BR'><head><meta charset='UTF-8'><title>Estação Meteorológica - Monitoramento</title>"
"<style>"
"body { font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 0; background: linear-gradient(135deg, #E3F2FD, #BBDEFB); color: #212121; min-height: 100vh; overflow-x: hidden; }"
".menu { background: linear-gradient(90deg, #0288D1, #03A9F4); display: flex; justify-content: center; padding: 15px; box-shadow: 0 4px 6px rgba(0,0,0,0.2); position: sticky; top: 0; z-index: 1000; }"
".menu button { margin: 0 10px; padding: 10px 20px; font-size: 16px; background: #03A9F4; color: #fff; border: none; border-radius: 25px; cursor: pointer; transition: all 0.3s ease; position: relative; overflow: hidden; }"
".menu button::after { content: ''; position: absolute; width: 0; height: 100%; top: 0; left: 0; background: rgba(255,255,255,0.2); transition: width 0.3s ease; }"
".menu button:hover::after { width: 100%; }"
".menu button:hover { background: #0288D1; transform: translateY(-2px); box-shadow: 0 2px 4px rgba(0,0,0,0.2); }"
".menu .datahora { font-size: 14px; color: #fff; position: absolute; right: 20px; transition: opacity 0.3s ease; }"
".section { display: none; padding: 20px; text-align: center; max-width: 900px; margin: 0 auto; animation: fadeIn 0.3s ease; }"
".section.active { display: block; }"
"@keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }"
".card { background: #fff; border-radius: 15px; box-shadow: 0 6px 12px rgba(0,0,0,0.1); padding: 20px; margin: 15px 0; transition: transform 0.2s ease; }"
".card:hover { transform: translateY(-2px); }"
".sensor-container { display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; }"
".sensor-box { background: linear-gradient(145deg, #F5F5F5, #E0E0E0); border-radius: 10px; padding: 15px; width: 160px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); transition: all 0.3s ease; }"
".sensor-box:hover { transform: scale(1.02); }"
".sensor-label { font-size: 14px; color: #757575; margin-bottom: 5px; }"
"span.value { font-size: 22px; font-weight: bold; color: #0288D1; }"
".form-group { display: flex; align-items: center; justify-content: center; gap: 15px; margin: 15px 0; }"
"input[type='number'] { padding: 10px 12px; border: 2px solid #B0BEC5; border-radius: 8px; font-size: 14px; transition: all 0.3s ease; outline: none; }"
"input[type='number']:focus { border-color: #03A9F4; box-shadow: 0 0 5px rgba(3,169,244,0.5); }"
"button { padding: 10px 20px; background: linear-gradient(45deg, #03A9F4, #0288D1); color: #fff; border: none; border-radius: 8px; cursor: pointer; transition: all 0.3s ease; }"
"button:hover { background: linear-gradient(45deg, #0288D1, #01579B); transform: translateY(-2px); box-shadow: 0 4px 8px rgba(0,0,0,0.2); }"
"button:active { transform: translateY(0); box-shadow: 0 2px 4px rgba(0,0,0,0.2); }"
".alert-status { font-size: 16px; font-weight: bold; margin-top: 10px; color: #D32F2F; animation: pulse 1.5s infinite; }"
"@keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.6; } 100% { opacity: 1; } }"
"#alerta_iframe { position: fixed; bottom: 0; left: 0; width: 100%; height: 40px; border: none; background: linear-gradient(90deg, #FFEBEE, #EF9A9A); z-index: 1000; box-shadow: 0 -2px 4px rgba(0,0,0,0.1); }"
".fab { display: inline-block; width: 60px; height: 60px; background: linear-gradient(45deg, #03A9F4, #0288D1); color: #fff; border-radius: 50%; text-align: center; line-height: 60px; font-size: 24px; box-shadow: 0 6px 12px rgba(0,0,0,0.2); position: absolute; right: 20px; bottom: 20px; cursor: pointer; transition: all 0.3s ease; animation: bounce 2s infinite; }"
".fab:hover { background: linear-gradient(45deg, #0288D1, #01579B); transform: scale(1.1); animation: none; }"
"@keyframes bounce { 0%, 20%, 50%, 80%, 100% { transform: translateY(0); } 40% { transform: translateY(-5px); } 60% { transform: translateY(-2px); } }"
".active-section { background: #01579B !important; }"
"@media (max-width: 700px) { .menu { flex-direction: column; } .menu button { margin: 5px 0; } .sensor-container { flex-direction: column; align-items: center; } .sensor-box { width: 80%; } .grafico-box iframe { width: 100%; height: 150px; } .fab { right: 50%; transform: translateX(50%); bottom: 60px; } }"
"</style>"
"<script>"
"function showSection(sectionId) {"
"  document.querySelectorAll('.section').forEach(s => s.classList.remove('active'));"
"  document.getElementById(sectionId).classList.add('active');"
"  document.querySelectorAll('.menu button').forEach(btn => btn.classList.remove('active-section'));"
"  document.querySelector(`.menu button[onclick*=\"${sectionId}\"]`).classList.add('active-section');"
"}"
"function atualizar() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    document.getElementById('tmp1').innerText = parseFloat(data.temp_bmp).toFixed(1);"
"    document.getElementById('tmp2').innerText = parseFloat(data.temp_aht).toFixed(1);"
"    document.getElementById('umi').innerText = parseFloat(data.umidade).toFixed(1);"
"    document.getElementById('alt').innerText = parseFloat(data.altimetria).toFixed(0);"
"  }).catch(err => console.error(err));"
"  let data = new Date().toLocaleString('pt-BR', { timeZone: 'America/Sao_Paulo', hour12: true, hour: '2-digit', minute: '2-digit', day: '2-digit', month: '2-digit', year: 'numeric' }).replace(',', '');"
"  document.getElementById('datahora').innerText = data;"
"}"
"function atualizarOffsets() {"
"  fetch('/get_offsets').then(res => res.json()).then(data => {"
"    document.getElementById('offset_bmp_val').innerText = data.offset_bmp.toFixed(1) + ' °C';"
"    document.getElementById('offset_aht_val').innerText = data.offset_aht.toFixed(1) + ' °C';"
"    document.getElementById('offset_umi_val').innerText = data.offset_umi.toFixed(1) + ' %';"
"    document.getElementById('offset_alt_val').innerText = data.offset_alt.toFixed(0) + ' m';"
"  }).catch(err => console.error(err));"
"}"
"function salvarOffset(type, value) {"
"  fetch(`/set_offset?type=${type}&value=${value}`)"
"    .then(res => res.text())"
"    .then(msg => alert(msg))"
"    .catch(err => console.error('Erro no fetch: ' + err));"
"}"
"function salvarAlerta(type, value) {"
"  fetch(`/set_alert?type=${type}&value=${value}`)"
"    .then(res => res.text())"
"    .then(msg => alert(msg))"
"    .catch(err => console.error('Erro no fetch: ' + err));"
"}"
"function atualizarAlertas() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    const tempBMP = parseFloat(data.temp_bmp);"
"    const tempAHT = parseFloat(data.temp_aht);"
"    const umi = parseFloat(data.umidade);"
"    fetch('/get_alerts').then(res => res.json()).then(alerts => {"
"      const tempMin = alerts.alerta_temp_min;"
"      const tempMax = alerts.alerta_temp_max;"
"      const umiMin = alerts.alerta_umi_minima;"
"      const umiMax = alerts.alerta_umi_maxima;"
"      let tempStatus = 'Temperatura Normal';"
"      if (tempBMP < tempMin || tempAHT < tempMin) tempStatus = 'Alerta: Temperatura Muito Baixa';"
"      else if (tempBMP > tempMax || tempAHT > tempMax) tempStatus = 'Alerta: Temperatura Muito Alta';"
"      let umiStatus = 'Umidade Normal';"
"      if (umi < umiMin) umiStatus = 'Alerta: Umidade Muito Baixa';"
"      else if (umi > umiMax) umiStatus = 'Alerta: Umidade Muito Alta';"
"      document.getElementById('temp_status').innerText = tempStatus;"
"      document.getElementById('umi_status').innerText = umiStatus;"
"    });"
"  }).catch(err => console.error(err));"
"}"
"window.onload = () => {"
"  showSection('sensores');"
"  atualizar();"
"  atualizarOffsets();"
"  atualizarAlertas();"
"  setInterval(atualizar, 2000);"
"  setInterval(atualizarOffsets, 2000);"
"  setInterval(atualizarAlertas, 2000);"
"};"
"</script>"
"</head><body>"
"<div class='menu'>"
"<button onclick=\"showSection('sensores')\">Sensores</button>"
"<button onclick=\"showSection('graficos')\">Gráficos</button>"
"<button onclick=\"showSection('calibracao')\">Calibração</button>"
"<button onclick=\"showSection('alertas')\">Alertas</button>"
"<div class='datahora' id='datahora'>--:-- --, --/--/----</div>"
"</div>"
"<div id='sensores' class='section active'>"
"<h2>Sensores</h2>"
"<div class='card sensor-container'>"
"<div class='sensor-box'><div class='sensor-label'>Temp BMP280</div><div><span id='tmp1' class='value'>--</span>°C</div></div>"
"<div class='sensor-box'><div class='sensor-label'>Temp AHT20</div><div><span id='tmp2' class='value'>--</span>°C</div></div>"
"<div class='sensor-box'><div class='sensor-label'>Umidade</div><div><span id='umi' class='value'>--</span>%</div></div>"
"<div class='sensor-box'><div class='sensor-label'>Altitude</div><div><span id='alt' class='value'>--</span>m</div></div>"
"</div>"
"</div>"
"<div id='graficos' class='section'>"
"<h2>Gráficos</h2>"
"<div class='card'>"
"<div class='grafico-box'><iframe src='/grafico1'></iframe><p>Temp BMP280</p></div>"
"<div class='grafico-box'><iframe src='/grafico2'></iframe><p>Temp AHT20</p></div>"
"<div class='grafico-box'><iframe src='/grafico3'></iframe><p>Umidade</p></div>"
"</div>"
"</div>"
"<div id='calibracao' class='section'>"
"<h2>Calibração</h2>"
"<div class='card'>"
"<div class='form-group'><p>Offset Temp BMP280: <span id='offset_bmp_val'>0.0 °C</span></p><label>Offset (°C): <input type='number' id='offset_bmp_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('bmp', document.getElementById('offset_bmp_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Temp AHT20: <span id='offset_aht_val'>0.0 °C</span></p><label>Offset (°C): <input type='number' id='offset_aht_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('aht', document.getElementById('offset_aht_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Umidade: <span id='offset_umi_val'>0.0 %</span></p><label>Offset (%): <input type='number' id='offset_umi_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('umi', document.getElementById('offset_umi_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Altitude: <span id='offset_alt_val'>0.0 m</span></p><label>Offset (m): <input type='number' id='offset_alt_input' step='1' value='0'></label><button onclick=\"salvarOffset('alt', document.getElementById('offset_alt_input').value)\">Salvar</button></div>"
"</div>"
"</div>"
"<div id='alertas' class='section'>"
"<h2>Alertas</h2>"
"<div class='card'>"
"<p>Temperatura Status: <span id='temp_status' class='alert-status'>--</span></p>"
"<div class='form-group'><label>Temp Min (°C): <input type='number' id='alerta_temp_min_input' step='1' value='0'></label><button onclick=\"salvarAlerta('temp_min', document.getElementById('alerta_temp_min_input').value)\">Salvar</button></div>"
"<div class='form-group'><label>Temp Max (°C): <input type='number' id='alerta_temp_max_input' step='1' value='50'></label><button onclick=\"salvarAlerta('temp_max', document.getElementById('alerta_temp_max_input').value)\">Salvar</button></div>"
"<p>Umidade Status: <span id='umi_status' class='alert-status'>--</span></p>"
"<div class='form-group'><label>Umidade Min (%): <input type='number' id='alerta_umi_min_input' step='1' value='10'></label><button onclick=\"salvarAlerta('umi_min', document.getElementById('alerta_umi_min_input').value)\">Salvar</button></div>"
"<div class='form-group'><label>Umidade Max (%): <input type='number' id='alerta_umi_max_input' step='1' value='80'></label><button onclick=\"salvarAlerta('umi_max', document.getElementById('alerta_umi_max_input').value)\">Salvar</button></div>"
"</div>"
"</div>"
"<iframe id='alerta_iframe' src='/alertas'></iframe>"
"<div class='fab' onclick=\"showSection('sensores')\">+</div>"
"</body></html>";

// HTML para gráfico1 (Temp. BMP280) (~400 bytes)
const char HTML_GRAFICO1[] =
"<!DOCTYPE html><html><body><canvas id='grafico1' width='400' height='200'></canvas><script>"
"const maxPontos = 8; let historico = [];"
"function desenharGrafico() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    let val = parseFloat(data.temp_bmp);"
"    if (historico.length >= maxPontos) historico.shift();"
"    historico.push(val);"
"    const c = document.getElementById('grafico1');"
"    const ctx = c.getContext('2d');"
"    ctx.clearRect(0, 0, c.width, c.height);"
"    if (historico.length < 2) return;"
"    const w = c.width, h = c.height;"
"    ctx.strokeStyle = 'rgba(224, 224, 224, 0.5)'; ctx.lineWidth = 0.5;"
"    for (let i = 0; i <= 4; i++) { let y = h - (i * h / 4); ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke(); }"
"    for (let i = 0; i <= maxPontos; i++) { let x = (i * w) / (maxPontos - 1); ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke(); }"
"    ctx.fillStyle = '#757575'; ctx.font = '12px Arial';"
"    ctx.fillText('0°C', 5, h-5); ctx.fillText('25°C', 5, h*3/4-5); ctx.fillText('50°C', 5, h/2-5); ctx.fillText('75°C', 5, h/4-5); ctx.fillText('100°C', 5, 5);"
"    let gradient = ctx.createLinearGradient(0, 0, 0, h); gradient.addColorStop(0, '#0288D1'); gradient.addColorStop(0.5, '#03A9F4'); gradient.addColorStop(1, '#B3E5FC');"
"    ctx.strokeStyle = gradient; ctx.lineWidth = 2;"
"    ctx.beginPath();"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y); });"
"    ctx.stroke();"
"    ctx.fillStyle = '#0288D1';"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; ctx.beginPath(); ctx.arc(x, y, 3, 0, Math.PI * 2); ctx.fill(); });"
"  });"
"}"
"setInterval(desenharGrafico, 2000);"
"</script></body></html>";

// HTML para gráfico2 (Temp. AHT20) (~400 bytes)
const char HTML_GRAFICO2[] =
"<!DOCTYPE html><html><body><canvas id='grafico2' width='400' height='200'></canvas><script>"
"const maxPontos = 8; let historico = [];"
"function desenharGrafico() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    let val = parseFloat(data.temp_aht);"
"    if (historico.length >= maxPontos) historico.shift();"
"    historico.push(val);"
"    const c = document.getElementById('grafico2');"
"    const ctx = c.getContext('2d');"
"    ctx.clearRect(0, 0, c.width, c.height);"
"    if (historico.length < 2) return;"
"    const w = c.width, h = c.height;"
"    ctx.strokeStyle = 'rgba(224, 224, 224, 0.5)'; ctx.lineWidth = 0.5;"
"    for (let i = 0; i <= 4; i++) { let y = h - (i * h / 4); ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke(); }"
"    for (let i = 0; i <= maxPontos; i++) { let x = (i * w) / (maxPontos - 1); ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke(); }"
"    ctx.fillStyle = '#757575'; ctx.font = '12px Arial';"
"    ctx.fillText('0°C', 5, h-5); ctx.fillText('25°C', 5, h*3/4-5); ctx.fillText('50°C', 5, h/2-5); ctx.fillText('75°C', 5, h/4-5); ctx.fillText('100°C', 5, 5);"
"    let gradient = ctx.createLinearGradient(0, 0, 0, h); gradient.addColorStop(0, '#4CAF50'); gradient.addColorStop(0.5, '#66BB6A'); gradient.addColorStop(1, '#C8E6C9');"
"    ctx.strokeStyle = gradient; ctx.lineWidth = 2;"
"    ctx.beginPath();"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y); });"
"    ctx.stroke();"
"    ctx.fillStyle = '#4CAF50';"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; ctx.beginPath(); ctx.arc(x, y, 3, 0, Math.PI * 2); ctx.fill(); });"
"  });"
"}"
"setInterval(desenharGrafico, 2000);"
"</script></body></html>";

// HTML para gráfico3 (Umidade) (~400 bytes)
const char HTML_GRAFICO3[] =
"<!DOCTYPE html><html><body><canvas id='grafico3' width='400' height='200'></canvas><script>"
"const maxPontos = 8; let historico = [];"
"function desenharGrafico() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    let val = parseFloat(data.umidade);"
"    if (historico.length >= maxPontos) historico.shift();"
"    historico.push(val);"
"    const c = document.getElementById('grafico3');"
"    const ctx = c.getContext('2d');"
"    ctx.clearRect(0, 0, c.width, c.height);"
"    if (historico.length < 2) return;"
"    const w = c.width, h = c.height;"
"    ctx.strokeStyle = 'rgba(224, 224, 224, 0.5)'; ctx.lineWidth = 0.5;"
"    for (let i = 0; i <= 4; i++) { let y = h - (i * h / 4); ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke(); }"
"    for (let i = 0; i <= maxPontos; i++) { let x = (i * w) / (maxPontos - 1); ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke(); }"
"    ctx.fillStyle = '#757575'; ctx.font = '12px Arial';"
"    ctx.fillText('0%', 5, h-5); ctx.fillText('25%', 5, h*3/4-5); ctx.fillText('50%', 5, h/2-5); ctx.fillText('75%', 5, h/4-5); ctx.fillText('100%', 5, 5);"
"    let gradient = ctx.createLinearGradient(0, 0, 0, h); gradient.addColorStop(0, '#FF9800'); gradient.addColorStop(0.5, '#F57C00'); gradient.addColorStop(1, '#FFE0B2');"
"    ctx.strokeStyle = gradient; ctx.lineWidth = 2;"
"    ctx.beginPath();"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y); });"
"    ctx.stroke();"
"    ctx.fillStyle = '#FF9800';"
"    historico.forEach((v, i) => { const x = (i * w) / (maxPontos - 1); const y = h - (v / 100) * h; ctx.beginPath(); ctx.arc(x, y, 3, 0, Math.PI * 2); ctx.fill(); });"
"  });"
"}"
"setInterval(desenharGrafico, 2000);"
"</script></body></html>";
//--------------------------------------------------------------
// HTML para calibração (~400 bytes)
const char HTML_CALIBRACAO[] =
"<!DOCTYPE html><html lang='pt-BR'><head><meta charset='UTF-8'><title>Calibração</title></head><body>"
"<h2>Calibração</h2>"
"<div class='card'>"
"<div class='form-group'><p>Offset Temp BMP280: <span id='offset_bmp_val'>0.0 °C</span></p><label>Offset (°C): <input type='number' id='offset_bmp_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('bmp', document.getElementById('offset_bmp_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Temp AHT20: <span id='offset_aht_val'>0.0 °C</span></p><label>Offset (°C): <input type='number' id='offset_aht_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('aht', document.getElementById('offset_aht_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Umidade: <span id='offset_umi_val'>0.0 %</span></p><label>Offset (%): <input type='number' id='offset_umi_input' step='0.1' value='0'></label><button onclick=\"salvarOffset('umi', document.getElementById('offset_umi_input').value)\">Salvar</button></div>"
"<div class='form-group'><p>Offset Altitude: <span id='offset_alt_val'>0.0 m</span></p><label>Offset (m): <input type='number' id='offset_alt_input' step='1' value='0'></label><button onclick=\"salvarOffset('alt', document.getElementById('offset_alt_input').value)\">Salvar</button></div>"
"</div>"
"</body></html>";

// HTML para alertas (~400 bytes)
const char HTML_ALERTAS[] =
"<!DOCTYPE html><html lang='pt-BR'><head><meta charset='UTF-8'><title>Alertas</title></head><body>"
"<style>"
"body { margin: 0; padding: 0; font-family: 'Segoe UI', Arial; text-align: center; }"
"#alerta_status { font-size: 16px; font-weight: bold; padding: 10px; color: #D32F2F; animation: pulse 1.5s infinite; }"
"@keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.6; } 100% { opacity: 1; } }"
"</style>"
"<script>"
"function atualizarAlertas() {"
"  fetch('/update').then(res => res.json()).then(data => {"
"    const tempBMP = parseFloat(data.temp_bmp);"
"    const tempAHT = parseFloat(data.temp_aht);"
"    const umi = parseFloat(data.umidade);"
"    fetch('/get_alerts').then(res => res.json()).then(alerts => {"
"      const tempMin = alerts.alerta_temp_min;"
"      const tempMax = alerts.alerta_temp_max;"
"      const umiMin = alerts.alerta_umi_minima;"
"      const umiMax = alerts.alerta_umi_maxima;"
"      let tempStatus = 'Temperatura Normal';"
"      if (tempBMP < tempMin || tempAHT < tempMin) tempStatus = 'Alerta: Temperatura Muito Baixa';"
"      else if (tempBMP > tempMax || tempAHT > tempMax) tempStatus = 'Alerta: Temperatura Muito Alta';"
"      let umiStatus = 'Umidade Normal';"
"      if (umi < umiMin) umiStatus = 'Alerta: Umidade Muito Baixa';"
"      else if (umi > umiMax) umiStatus = 'Alerta: Umidade Muito Alta';"
"      const alertaStatus = document.getElementById('alerta_status');"
"      alertaStatus.innerText = tempStatus + ' | ' + umiStatus;"
"    });"
"  }).catch(err => console.error(err));"
"}"
"setInterval(atualizarAlertas, 2000);"
"</script>"
"<div id='alerta_status'></div>"
"</body></html>";

struct http_state {
    char response[4*4096]; // Buffer aumentado
    size_t len;
    size_t sent;
};

static err_t http_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    struct http_state *hs = (struct http_state *)arg;
    hs->sent += len;
    printf("Enviado: %u de %u bytes\n", hs->sent, hs->len);
    if (hs->sent >= hs->len) {
        printf("Conexão fechada\n");
        tcp_close(tpcb);
        free(hs);
    }
    return ERR_OK;
}

static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    char *req = (char *)p->payload;
    struct http_state *hs = malloc(sizeof(struct http_state));
    if (!hs) {
        pbuf_free(p);
        tcp_close(tpcb);
        printf("Erro: Falha na alocação de memória\n");
        return ERR_MEM;
    }
    hs->sent = 0;

    if (strstr(req, "GET /update")) {
        char json[2*256];
        int json_len = snprintf(json, sizeof(json),
            "{\"temp_bmp\":\"%s\",\"temp_aht\":\"%s\",\"umidade\":\"%s\",\"altimetria\":\"%s\"}",
            str_tmp1, str_tmp2, str_umi, str_alt);
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            json_len, json);
    } else if (strstr(req, "GET /grafico1")) {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_GRAFICO1), HTML_GRAFICO1);
    } else if (strstr(req, "GET /grafico2")) {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_GRAFICO2), HTML_GRAFICO2);
    } else if (strstr(req, "GET /grafico3")) {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_GRAFICO3), HTML_GRAFICO3);
    } else if (strstr(req, "GET /calibracao")) {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_CALIBRACAO), HTML_CALIBRACAO);
    } else if (strstr(req, "GET /alertas")) {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_ALERTAS), HTML_ALERTAS);
    } else if (strstr(req, "GET /get_offsets")) {
        char json[2*256];
        int json_len = snprintf(json, sizeof(json),
            "{\"offset_bmp\":%.1f,\"offset_aht\":%.1f,\"offset_umi\":%.1f,\"offset_alt\":%.0f}",
            offset_bmp, offset_aht, offset_umi, offset_alt);
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            json_len, json);
    } else if (strstr(req, "GET /set_offset")) {
        char *type_str = strstr(req, "type=");
        char *value_str = strstr(req, "value=");
        if (type_str && value_str) {
            type_str += 5;
            value_str += 6;
            char type[10], value[10];
            char *end_type = strchr(type_str, '&');
            if (end_type) *end_type = '\0';
            strncpy(type, type_str, sizeof(type) - 1);
            strncpy(value, value_str, sizeof(value) - 1);
            float new_value = atof(value);
            if (strcmp(type, "bmp") == 0) {
                offset_bmp = new_value;
                printf("Offset BMP atualizado: %.1f\n", offset_bmp);
            } else if (strcmp(type, "aht") == 0) {
                offset_aht = new_value;
                printf("Offset AHT atualizado: %.1f\n", offset_aht);
            } else if (strcmp(type, "umi") == 0) {
                offset_umi = new_value;
                printf("Offset UMI atualizado: %.1f\n", offset_umi);
            } else if (strcmp(type, "alt") == 0) {
                offset_alt = new_value;
                printf("Offset ALT atualizado: %.0f\n", offset_alt);
            } else {
                hs->len = snprintf(hs->response, sizeof(hs->response),
                    "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\nConnection: close\r\n\r\nErro!");
                tcp_arg(tpcb, hs);
                tcp_sent(tpcb, http_sent);
                tcp_write(tpcb, hs->response, hs->len, TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);
                pbuf_free(p);
                return ERR_OK;
            }
            const char *txt = "Configurado!";
            hs->len = snprintf(hs->response, sizeof(hs->response),
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                (int)strlen(txt), txt);
        } else {
            hs->len = snprintf(hs->response, sizeof(hs->response),
                "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\nConnection: close\r\n\r\nErro!");
        }
    } else if (strstr(req, "GET /get_alerts")) {
        char json[2*256];
        int json_len = snprintf(json, sizeof(json),
            "{\"alerta_temp_min\":%u,\"alerta_temp_max\":%u,\"alerta_umi_minima\":%u,\"alerta_umi_maxima\":%u}",
            alerta_temp_min, alerta_temp_max, alerta_umi_minima, alerta_umi_maxima);
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            json_len, json);
    } else if (strstr(req, "GET /set_alert")) {
        char *type_str = strstr(req, "type=");
        char *value_str = strstr(req, "value=");
        if (type_str && value_str) {
            type_str += 5;
            value_str += 6;
            char type[10], value[10];
            char *end_type = strchr(type_str, '&');
            if (end_type) *end_type = '\0';
            strncpy(type, type_str, sizeof(type) - 1);
            strncpy(value, value_str, sizeof(value) - 1);
            uint16_t new_value = atoi(value);
            if (strcmp(type, "temp_min") == 0) {
                alerta_temp_min = new_value;
                printf("Alerta Temp Min atualizado: %u\n", alerta_temp_min);
            } else if (strcmp(type, "temp_max") == 0) {
                alerta_temp_max = new_value;
                printf("Alerta Temp Max atualizado: %u\n", alerta_temp_max);
            } else if (strcmp(type, "umi_min") == 0) {
                alerta_umi_minima = new_value;
                printf("Alerta Umi Min atualizado: %u\n", alerta_umi_minima);
            } else if (strcmp(type, "umi_max") == 0) {
                alerta_umi_maxima = new_value;
                printf("Alerta Umi Max atualizado: %u\n", alerta_umi_maxima);
            } else {
                hs->len = snprintf(hs->response, sizeof(hs->response),
                    "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\nConnection: close\r\n\r\nErro!");
                tcp_arg(tpcb, hs);
                tcp_sent(tpcb, http_sent);
                tcp_write(tpcb, hs->response, hs->len, TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);
                pbuf_free(p);
                return ERR_OK;
            }
            const char *txt = "Configurado!";
            hs->len = snprintf(hs->response, sizeof(hs->response),
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
                (int)strlen(txt), txt);
        } else {
            hs->len = snprintf(hs->response, sizeof(hs->response),
                "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\nConnection: close\r\n\r\nErro!");
        }
    } else {
        hs->len = snprintf(hs->response, sizeof(hs->response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
            (int)strlen(HTML_BODY), HTML_BODY);
    }

    if (hs->len >= sizeof(hs->response)) {
        printf("Erro: Buffer de resposta overflow! Tamanho: %u, Limite: %u\n", hs->len, sizeof(hs->response));
        hs->len = 0;
    }

    tcp_arg(tpcb, hs);
    tcp_sent(tpcb, http_sent);
    err_t write_err = tcp_write(tpcb, hs->response, hs->len, TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        printf("Erro no tcp_write: %d\n", write_err);
    }
    tcp_output(tpcb);
    pbuf_free(p);
    return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_recv);
    return ERR_OK;
}

static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb || tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao iniciar servidor HTTP\n");
        return;
    }
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
    printf("Servidor HTTP iniciado na porta 80\n");
}

#endif