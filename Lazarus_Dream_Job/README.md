# Lazarus Group "Dream Job" Emulation

This repository contains an emulation plan and payload source code to simulate the Lazarus Group's "Dream Job" campaign. The attack chain utilizes a trojanized Notepad++ plugin (`ComparePlus.dll`) to perform Early Bird APC Process Injection, followed by lateral movement, credential access, persistence, and network discovery using a simulated `SiteShooter` script.

## Repository Contents

* `lazarus_compareplus_emulation.yaml` - The CALDERA adversary profile (Emulation Plan).
* `payloads/malicious_compareplus.c` - The C source code for the trojanized DLL.

---

## Phase 1: Payload Compilation & Staging

The provided C file (`malicious_compareplus.c`) acts as the Stage 1 loader. It is designed to be executed via `rundll32.exe`. It creates a suspended `notepad.exe` process, injects a PowerShell command via Asynchronous Procedure Calls (APC), and resumes the thread. For safe emulation, this injected command simply drops a canary file (`injection_canary.txt`) to prove the injection succeeded.

### 1. Compile the DLL
You must compile the C code into a Windows DLL. You can do this from a Linux machine using MinGW, or from Windows using Visual Studio.

**Option A: Using MinGW (Linux/Kali/Parrot)**
```bash
x86_64-w64-mingw32-gcc -shared -o ComparePlus.dll payloads/malicious_compareplus.c

```

**Option B: Using Visual Studio Developer Command Prompt (Windows)**

```cmd
cl.exe /D_USRDLL /D_WINDLL payloads\malicious_compareplus.c /link /DLL /OUT:ComparePlus.dll

```

### 2. Host the DLL

CALDERA (and the target machine) needs a place to download this DLL during the initial execution step. Host it on your simulated C2 server (e.g., `192.168.56.100`).

Navigate to the directory where you compiled `ComparePlus.dll` and start a simple Python HTTP server on port `8085` (or whatever port you plan to define in your Fact Source):

```bash
python3 -m http.server 8085

```

---

## Phase 2: CALDERA Configuration

### 1. Create the Fact Source

The emulation plan uses dynamic variables (`#{lazarus.c2.url}` and `#{target.remote.node}`). You need to create a Fact Source file in CALDERA so it knows how to populate these variables during the operation.

On your CALDERA server, create a file named `lazarus_dream_job_sources.yml` inside your `~/caldera/data/sources/` directory with the following content (update the IP addresses to match your lab environment):

```yaml
- id: b2c3d4e5-6789-01fa-23bc-456789abcdef
  name: Lazarus Operation Dream Job Facts
  facts:
    - trait: lazarus.c2.url
      value: [http://192.168.56.100:8085](http://192.168.56.100:8085)
      score: 1
    - trait: target.remote.node
      value: 192.168.56.105
      score: 1

```

### 2. Install the Emulation Plan (Adversary Profile)

Move the `lazarus_compareplus_emulation.yaml` file from this repository into your CALDERA adversaries directory:

```bash
mv lazarus_compareplus_emulation.yaml ~/caldera/data/adversaries/

```

*(Note: If you are using a specific plugin like Stockpile, the path might be `~/caldera/plugins/stockpile/data/adversaries/`)*

### 3. Restart CALDERA

Restart your CALDERA server to ensure it parses the new YAML files correctly.

```bash
python3 server.py

```

---

## Phase 3: Execution & Validation

### 1. Deploy the Agent

Deploy a CALDERA agent (e.g., Sandcat) onto your Windows target machine (e.g., `192.168.56.105`) with high privileges (Administrator) to ensure all steps, particularly SAM dumping and Scheduled Tasks, can execute.

### 2. Run the Operation

1. Open the CALDERA Web UI and navigate to **Campaigns -> Operations**.
2. Click **Create Operation**.
3. Select **Lazarus_ComparePlus_Emulation** from the Adversary dropdown.
4. Expand **Advanced Options**.
5. Under the **Source** dropdown, select **Lazarus Operation Dream Job Facts**.
6. Click **Start**.

### 3. Validate Emulation Success

The operation is designed to run safely. You can manually verify the attack chain succeeded by checking for the following artifacts on the Windows target:

* **Process Injection Canary:** `C:\Windows\Temp\injection_canary.txt`
* **WMI Lateral Movement Canary:** `C:\Windows\Temp\wmi_canary.txt`
* **Registry Credentials Dump:** `C:\Windows\Temp\registry_creds.txt`
* **SAM/SYSTEM Hive Dumps:** `C:\Windows\Temp\sam.save` and `C:\Windows\Temp\system.save`
* **Persistence Canary:** `C:\Windows\Temp\persist_canary.txt`
* **SiteShooter Recon Data:** `C:\Windows\Temp\SiteShooter_results.txt`

### 4. Cleanup

To reset the environment for another run, delete the artifacts from the target machine:

```powershell
Remove-Item -Path C:\Windows\Temp\*.txt, C:\Windows\Temp\*.save, C:\Windows\Temp\ComparePlus.dll, C:\Windows\Temp\SiteShooter.ps1 -Force
schtasks /delete /tn "WindowsUpdateTask" /f

```

