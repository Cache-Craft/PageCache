async function simulate(){
 const algorithm=document.getElementById("algorithm").value;
 const cache_size=Number(document.getElementById("cache").value);
 const pages=document.getElementById("pages").value.split(",").map(x=>Number(x.trim())).filter(Number.isFinite);
 const out=document.getElementById("output");
 out.textContent="Running...";
 try{
  const r=await fetch("/api/simulate",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({algorithm,cache_size,pages})});
  const d=await r.json(); if(!r.ok)throw new Error(d.error);
  const m=d.metrics;
  document.getElementById("cards").innerHTML=[
   ["Selected",d.selected_algorithm],["Hits",m.hits],["Misses",m.misses],
   ["Hit Ratio",(m.hit_ratio*100).toFixed(2)+"%"],["Evictions",m.evictions],
   ["Avg Latency",m.avg_latency_ms.toFixed(2)+" ms"],["Working Set",m.working_set_size],
   ["Pollution",m.pollution_detected?"Detected":"No"]
  ].map(x=>`<div class="card"><small>${x[0]}</small><br><b>${x[1]}</b></div>`).join("");
  out.textContent=JSON.stringify(d,null,2); loadHistory();
 }catch(e){out.textContent="Error: "+e.message;out.className="error"}
}
async function loadHistory(){
 const r=await fetch("/api/history"); if(!r.ok)return; const rows=await r.json();
 document.getElementById("history").innerHTML=rows.map(x=>`<tr><td>${x.simulation_id}</td><td>${x.algorithm}</td><td>${(x.hit_ratio*100).toFixed(2)}%</td><td>${(x.miss_ratio*100).toFixed(2)}%</td><td>${x.avg_latency_ms} ms</td></tr>`).join("");
}
loadHistory();
