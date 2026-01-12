document.addEventListener('DOMContentLoaded', () => {
    // --- WebSocket Connection ---
    // In a real ESP32 environment, the gateway is the ESP32's IP.
    // For local testing, you might use 'ws://localhost:81'.
    const gateway = `ws://${window.location.hostname}/ws`;
    let websocket;

    function initWebSocket() {
        console.log('Trying to open a WebSocket connection...');
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
    }

    function onOpen(event) {
        console.log('Connection opened');
    }

    function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 2000);
    }

    function onMessage(event) {
        // Handle incoming data from the ESP32
        const data = JSON.parse(event.data);
        updateUI(data);
    }

    function sendMessage(message) {
        websocket.send(JSON.stringify(message));
    }

    // --- UI Element References ---
    const stateEl = document.getElementById('state');
    const batteryEl = document.getElementById('battery');
    const batteryPercentEl = document.getElementById('battery-percent');
    const padsReadyEl = document.getElementById('pads-ready');
    const padsGridEl = document.getElementById('pads-grid');

    const armBtn = document.getElementById('btn-arm');
    const disarmBtn = document.getElementById('btn-disarm');
    const rangeOpenBtn = document.getElementById('btn-range-open');
    const rangeCloseBtn = document.getElementById('btn-range-close');

    // Modals
    const armModal = document.getElementById('arm-modal');
    const pinModal = document.getElementById('pin-modal');
    const armConfirmInput = document.getElementById('arm-confirm-input');
    const pinInput = document.getElementById('pin-input');

    // --- UI Update Logic ---
    function updateUI(data) {
        stateEl.textContent = data.state;
        batteryEl.textContent = data.battery.voltage.toFixed(1);
        batteryPercentEl.textContent = data.battery.percentage;

        let padsReady = 0;
        padsGridEl.innerHTML = ''; // Clear the grid
        for (let i = 0; i < data.pads.length; i++) {
            const pad = document.createElement('div');
            pad.className = 'pad';
            pad.textContent = `Pad ${i + 1}`;

            if (data.pads[i] === 'CONTINUITY_OK') {
                pad.classList.add('continuity-ok');
                padsReady++;
            }

            if (data.state === 'ARMED') {
                pad.classList.add('armed');
            }

            pad.addEventListener('click', () => {
                if(data.state === 'ARMED' && data.pads[i] === 'CONTINUITY_OK') {
                    firePad(i);
                }
            });
            padsGridEl.appendChild(pad);
        }
        padsReadyEl.textContent = padsReady;
    }

    // --- Event Listeners ---
    armBtn.addEventListener('click', () => {
        armModal.style.display = 'block';
        armConfirmInput.focus();
    });

    disarmBtn.addEventListener('click', () => {
        sendMessage({ command: 'disarm' });
    });

    // ... Add listeners for range open/close with PIN modal ...

    document.getElementById('btn-confirm-arm').addEventListener('click', () => {
        if (armConfirmInput.value.toUpperCase() === 'ARM') {
            sendMessage({ command: 'arm' });
            closeArmModal();
        }
    });

    document.getElementById('btn-cancel-arm').addEventListener('click', closeArmModal);

    function closeArmModal() {
        armModal.style.display = 'none';
        armConfirmInput.value = '';
    }

    function firePad(padIndex) {
        sendMessage({ command: 'fire', pad: padIndex });
    }

    // --- Initial Setup ---
    initWebSocket();
});
