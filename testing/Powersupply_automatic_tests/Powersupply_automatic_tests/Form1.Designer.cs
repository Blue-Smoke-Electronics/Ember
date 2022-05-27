
namespace Powersupply_automatic_tests
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonSerial = new System.Windows.Forms.Button();
            this.isConnectedBlueSmoek = new System.Windows.Forms.RadioButton();
            this.isConnectedKonradPSU = new System.Windows.Forms.RadioButton();
            this.isConnectedKoradLoad = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonVoltageTest = new System.Windows.Forms.Button();
            this.buttonCurrentTest = new System.Windows.Forms.Button();
            this.voltMessTextbox = new System.Windows.Forms.TextBox();
            this.currentMessTextbox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.loadCurrentTextbox = new System.Windows.Forms.TextBox();
            this.loadVoltageTextbox = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.randomButton = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonSerial
            // 
            this.buttonSerial.Location = new System.Drawing.Point(21, 24);
            this.buttonSerial.Name = "buttonSerial";
            this.buttonSerial.Size = new System.Drawing.Size(138, 23);
            this.buttonSerial.TabIndex = 0;
            this.buttonSerial.Text = "Connect serial";
            this.buttonSerial.UseVisualStyleBackColor = true;
            this.buttonSerial.Click += new System.EventHandler(this.button1_Click);
            // 
            // isConnectedBlueSmoek
            // 
            this.isConnectedBlueSmoek.AutoCheck = false;
            this.isConnectedBlueSmoek.AutoSize = true;
            this.isConnectedBlueSmoek.Location = new System.Drawing.Point(21, 75);
            this.isConnectedBlueSmoek.Name = "isConnectedBlueSmoek";
            this.isConnectedBlueSmoek.Size = new System.Drawing.Size(79, 17);
            this.isConnectedBlueSmoek.TabIndex = 1;
            this.isConnectedBlueSmoek.TabStop = true;
            this.isConnectedBlueSmoek.Text = "BlueSmoke";
            this.isConnectedBlueSmoek.UseVisualStyleBackColor = true;
            // 
            // isConnectedKonradPSU
            // 
            this.isConnectedKonradPSU.AutoCheck = false;
            this.isConnectedKonradPSU.AutoSize = true;
            this.isConnectedKonradPSU.Location = new System.Drawing.Point(21, 98);
            this.isConnectedKonradPSU.Name = "isConnectedKonradPSU";
            this.isConnectedKonradPSU.Size = new System.Drawing.Size(78, 17);
            this.isConnectedKonradPSU.TabIndex = 2;
            this.isConnectedKonradPSU.TabStop = true;
            this.isConnectedKonradPSU.Text = "Korad PSU";
            this.isConnectedKonradPSU.UseVisualStyleBackColor = true;
            // 
            // isConnectedKoradLoad
            // 
            this.isConnectedKoradLoad.AutoCheck = false;
            this.isConnectedKoradLoad.AutoSize = true;
            this.isConnectedKoradLoad.Location = new System.Drawing.Point(21, 121);
            this.isConnectedKoradLoad.Name = "isConnectedKoradLoad";
            this.isConnectedKoradLoad.Size = new System.Drawing.Size(80, 17);
            this.isConnectedKoradLoad.TabIndex = 3;
            this.isConnectedKoradLoad.TabStop = true;
            this.isConnectedKoradLoad.Text = "Korad Load";
            this.isConnectedKoradLoad.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 59);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Connected:";
            // 
            // buttonVoltageTest
            // 
            this.buttonVoltageTest.Location = new System.Drawing.Point(301, 24);
            this.buttonVoltageTest.Name = "buttonVoltageTest";
            this.buttonVoltageTest.Size = new System.Drawing.Size(139, 23);
            this.buttonVoltageTest.TabIndex = 5;
            this.buttonVoltageTest.Text = "Voltage test";
            this.buttonVoltageTest.UseVisualStyleBackColor = true;
            this.buttonVoltageTest.Click += new System.EventHandler(this.buttonVoltageTest_Click);
            // 
            // buttonCurrentTest
            // 
            this.buttonCurrentTest.Location = new System.Drawing.Point(301, 59);
            this.buttonCurrentTest.Name = "buttonCurrentTest";
            this.buttonCurrentTest.Size = new System.Drawing.Size(139, 23);
            this.buttonCurrentTest.TabIndex = 6;
            this.buttonCurrentTest.Text = "Current Test";
            this.buttonCurrentTest.UseVisualStyleBackColor = true;
            this.buttonCurrentTest.Click += new System.EventHandler(this.buttonCurrentTest_Click);
            // 
            // voltMessTextbox
            // 
            this.voltMessTextbox.Location = new System.Drawing.Point(21, 201);
            this.voltMessTextbox.Name = "voltMessTextbox";
            this.voltMessTextbox.Size = new System.Drawing.Size(100, 20);
            this.voltMessTextbox.TabIndex = 7;
            // 
            // currentMessTextbox
            // 
            this.currentMessTextbox.Location = new System.Drawing.Point(21, 227);
            this.currentMessTextbox.Name = "currentMessTextbox";
            this.currentMessTextbox.Size = new System.Drawing.Size(100, 20);
            this.currentMessTextbox.TabIndex = 8;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(127, 208);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(25, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Volt";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(127, 230);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Current";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(39, 185);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(25, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "pbp";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(39, 257);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(66, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Konrad PSU";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(127, 302);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 13);
            this.label6.TabIndex = 15;
            this.label6.Text = "Current";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(127, 280);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(25, 13);
            this.label7.TabIndex = 14;
            this.label7.Text = "Volt";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(21, 299);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(100, 20);
            this.textBox3.TabIndex = 13;
            this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(21, 273);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(100, 20);
            this.textBox4.TabIndex = 12;
            this.textBox4.TextChanged += new System.EventHandler(this.textBox4_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(39, 333);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(68, 13);
            this.label8.TabIndex = 21;
            this.label8.Text = "Konrad Load";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(127, 378);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 13);
            this.label9.TabIndex = 20;
            this.label9.Text = "Current";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(127, 356);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(25, 13);
            this.label10.TabIndex = 19;
            this.label10.Text = "Volt";
            // 
            // loadCurrentTextbox
            // 
            this.loadCurrentTextbox.Location = new System.Drawing.Point(21, 375);
            this.loadCurrentTextbox.Name = "loadCurrentTextbox";
            this.loadCurrentTextbox.Size = new System.Drawing.Size(100, 20);
            this.loadCurrentTextbox.TabIndex = 18;
            // 
            // loadVoltageTextbox
            // 
            this.loadVoltageTextbox.Location = new System.Drawing.Point(21, 349);
            this.loadVoltageTextbox.Name = "loadVoltageTextbox";
            this.loadVoltageTextbox.Size = new System.Drawing.Size(100, 20);
            this.loadVoltageTextbox.TabIndex = 17;
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // randomButton
            // 
            this.randomButton.Location = new System.Drawing.Point(301, 98);
            this.randomButton.Name = "randomButton";
            this.randomButton.Size = new System.Drawing.Size(139, 23);
            this.randomButton.TabIndex = 22;
            this.randomButton.Text = "Test random";
            this.randomButton.UseVisualStyleBackColor = true;
            this.randomButton.Click += new System.EventHandler(this.randomButton_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(301, 257);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 23;
            this.button1.Text = "Efficensy";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_3);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.randomButton);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.loadCurrentTextbox);
            this.Controls.Add(this.loadVoltageTextbox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.currentMessTextbox);
            this.Controls.Add(this.voltMessTextbox);
            this.Controls.Add(this.buttonCurrentTest);
            this.Controls.Add(this.buttonVoltageTest);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.isConnectedKoradLoad);
            this.Controls.Add(this.isConnectedKonradPSU);
            this.Controls.Add(this.isConnectedBlueSmoek);
            this.Controls.Add(this.buttonSerial);
            this.Name = "Form1";
            this.Text = "Powersupply Tester";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSerial;
        private System.Windows.Forms.RadioButton isConnectedBlueSmoek;
        private System.Windows.Forms.RadioButton isConnectedKonradPSU;
        private System.Windows.Forms.RadioButton isConnectedKoradLoad;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonVoltageTest;
        private System.Windows.Forms.Button buttonCurrentTest;
        private System.Windows.Forms.TextBox voltMessTextbox;
        private System.Windows.Forms.TextBox currentMessTextbox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox loadCurrentTextbox;
        private System.Windows.Forms.TextBox loadVoltageTextbox;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button randomButton;
        private System.Windows.Forms.Button button1;
    }
}

