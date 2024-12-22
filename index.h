<!DOCTYPE html>
<html lang="ar" dir="rtl">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>اختبار المشتقات</title>
  <link href="https://fonts.googleapis.com/css2?family=Amiri&family=Tajawal:wght@400;700&display=swap" rel="stylesheet">
  <style>
    body {
      font-family: 'Tajawal', sans-serif;
      background-color: #f0f4f8;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
    }
    .container {
      text-align: center;
      background-color: #ffffff;
      border-radius: 10px;
      box-shadow: 0 4px 10px rgba(0, 0, 0, 0.15);
      padding: 20px;
      width: 90%;
      max-width: 500px;
    }
    .header {
      font-size: 28px;
      font-weight: bold;
      margin-bottom: 10px;
      color: #007bff;
    }
    .timer {
      font-size: 18px;
      color: #ff5722;
      font-weight: bold;
      margin-bottom: 10px;
    }
    .score {
      font-size: 18px;
      color: #333;
      margin-bottom: 15px;
    }
    .function-box {
      font-size: 24px;
      font-family: 'Amiri', serif;
      font-weight: bold;
      margin-bottom: 20px;
      color: #0056b3;
    }
    .options {
      display: flex;
      flex-direction: column;
      gap: 10px;
    }
    .button {
      padding: 10px;
      font-size: 18px;
      border: none;
      border-radius: 5px;
      background-color: #bbdefb;
      cursor: pointer;
      transition: background-color 0.3s, transform 0.2s;
      font-family: 'Tajawal', sans-serif;
    }
    .button:hover {
      background-color: #90caf9;
      transform: scale(1.05);
    }
    .button.correct {
      background-color: #d4edda;
      border: 2px solid #28a745;
    }
    .button.wrong {
      background-color: #f8d7da;
      border: 2px solid #dc3545;
    }
    .explanation {
      margin-top: 15px;
      font-size: 16px;
      color: #555;
    }
    .next-btn {
      margin-top: 20px;
      padding: 10px 20px;
      font-size: 16px;
      background-color: #007bff;
      color: #fff;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      display: none;
    }
    .next-btn:hover {
      background-color: #0056b3;
    }
    .controls {
      margin: 10px 0;
    }
    .control-btn {
      margin: 0 5px;
      padding: 5px 10px;
      font-size: 14px;
      background-color: #007bff;
      color: #fff;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    .control-btn:hover {
      background-color: #0056b3;
    }
    .report {
      font-size: 18px;
      color: #333;
      margin-top: 20px;
      text-align: center;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">اختبار المشتقات</div>
    <div class="timer" id="timer">الوقت المتبقي: 60 ثانية</div>
    <div class="score" id="score">النقاط: 0</div>
    <div class="controls">
      <button class="control-btn" id="pause-timer">إيقاف المؤقت</button>
      <button class="control-btn" id="resume-timer" disabled>استئناف المؤقت</button>
      <button class="control-btn" id="training-mode">وضع التدريب</button>
    </div>
    <div class="function-box" id="function-box">f(x) = x²</div>
    <div class="options" id="options"></div>
    <div class="explanation" id="explanation"></div>
    <button class="next-btn" id="next-btn">السؤال التالي</button>
  </div>

  <script>
    const functionBox = document.getElementById("function-box");
    const optionsContainer = document.getElementById("options");
    const nextButton = document.getElementById("next-btn");
    const timerDisplay = document.getElementById("timer");
    const scoreDisplay = document.getElementById("score");
    const explanationDisplay = document.getElementById("explanation");
    const pauseTimerBtn = document.getElementById("pause-timer");
    const resumeTimerBtn = document.getElementById("resume-timer");
    const trainingModeBtn = document.getElementById("training-mode");

    let currentFunction = {};
    let timer;
    let timeLeft = 60;
    let timerPaused = false;
    let score = 0;
    let isTrainingMode = false;

    // الأصوات
    const correctSound = new Audio('https://cdn.pixabay.com/download/audio/2022/03/15/audio_4c40cf06e8.mp3');
    const wrongSound = new Audio('https://cdn.pixabay.com/download/audio/2022/03/15/audio_64d00ffdef.mp3');
    const timerEndSound = new Audio('https://cdn.pixabay.com/download/audio/2023/04/22/audio_f2812c71cb.mp3');

    // دالة لتوليد دالة عشوائية
    function generateRandomFunction() {
      const functions = [
        { fn: "x²", derivative: "2x", explanation: "مشتقة x² هي 2x باستخدام قاعدة القوة." },
        { fn: "x³", derivative: "3x²", explanation: "مشتقة x³ هي 3x² باستخدام قاعدة القوة." },
        { fn: "1/x", derivative: "-1/x²", explanation: "مشتقة 1/x هي -1/x² باستخدام قاعدة القسمة." }
      ];
      return functions[Math.floor(Math.random() * functions.length)];
    }

    // تحميل سؤال جديد
    function loadNewFunction() {
      clearInterval(timer);
      timeLeft = 60;
      updateTimer();
      startTimer();

      explanationDisplay.textContent = '';
      optionsContainer.innerHTML = '';
      nextButton.style.display = 'none';

      currentFunction = generateRandomFunction();
      functionBox.textContent = `f(𝑥) = ${currentFunction.fn}`;

      const options = generateOptions(currentFunction.derivative);
      options.forEach(option => {
        const button = document.createElement('button');
        button.textContent = option;
        button.classList.add('button');
        button.addEventListener('click', () => handleOptionClick(button, option));
        optionsContainer.appendChild(button);
      });
    }

    // إدارة الإجابة
    function handleOptionClick(button, selected) {
      clearInterval(timer);

      const buttons = document.querySelectorAll('.button');
      buttons.forEach(btn => btn.disabled = true);

      if (selected === currentFunction.derivative) {
        button.classList.add('correct');
        correctSound.play();
        score += 10;
        scoreDisplay.textContent = `النقاط: ${score}`;
      } else {
        button.classList.add('wrong');
        wrongSound.play();
        buttons.forEach(btn => {
          if (btn.textContent === currentFunction.derivative) {
            btn.classList.add('correct');
          }
        });
      }

      explanationDisplay.textContent = `الشرح: ${currentFunction.explanation}`;
      nextButton.style.display = 'block';
    }

    // إنشاء خيارات عشوائية
    function generateOptions(correct) {
      const allOptions = new Set([correct]);
      while (allOptions.size < 3) {
        const random = generateRandomFunction().derivative;
        allOptions.add(random);
      }
      return Array.from(allOptions).sort(() => Math.random() - 0.5);
    }

    // إدارة المؤقت
    function startTimer() {
      timerPaused = false;
      resumeTimerBtn.disabled = true;
      pauseTimerBtn.disabled = false;
      timer = setInterval(() => {
        if (!timerPaused) {
          timeLeft--;
          updateTimer();

          if (timeLeft <= 0) {
            clearInterval(timer);
            timerEndSound.play();
            showCorrectAnswer();
          }
        }
      }, 1000);
    }

    function updateTimer() {
      timerDisplay.textContent = `الوقت المتبقي: ${timeLeft} ثانية`;
    }

    function showCorrectAnswer() {
      const buttons = document.querySelectorAll('.button');
      buttons.forEach(btn => {
        btn.disabled = true;
        if (btn.textContent === currentFunction.derivative) {
          btn.classList.add('correct');
        }
      });
      nextButton.style.display = 'block';
    }

    pauseTimerBtn.addEventListener('click', () => {
      timerPaused = true;
      pauseTimerBtn.disabled = true;
      resumeTimerBtn.disabled = false;
    });

    resumeTimerBtn.addEventListener('click', () => {
      timerPaused = false;
      pauseTimerBtn.disabled = false;
      resumeTimerBtn.disabled = true;
    });

    nextButton.addEventListener('click', loadNewFunction);

    // تفعيل وضع التدريب
    trainingModeBtn.addEventListener('click', () => {
      isTrainingMode = !isTrainingMode;
      if (isTrainingMode) {
        trainingModeBtn.textContent = 'إيقاف التدريب';
        timerDisplay.style.display = 'none';
        pauseTimerBtn.style.display = 'none';
        resumeTimerBtn.style.display = 'none';
        clearInterval(timer);
      } else {
        trainingModeBtn.textContent = 'وضع التدريب';
        timerDisplay.style.display = 'block';
        pauseTimerBtn.style.display = 'inline-block';
        resumeTimerBtn.style.display = 'inline-block';
        startTimer();
      }
    });

    // البدء
    loadNewFunction();
  </script>
</body>
</html>
