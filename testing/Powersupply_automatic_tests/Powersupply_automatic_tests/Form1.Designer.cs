
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
            this.buttonSerial = new System.Windows.Forms.Button();
            this.isConnectedBlueSmoek = new System.Windows.Forms.RadioButton();
            this.isConnectedKonradPSU = new System.Windows.Forms.RadioButton();
            this.isConnectedKoradLoad = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonVoltageTest = new System.Windows.Forms.Button();
            this.buttonCurrentTest = new System.Windows.Forms.Button();
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
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
    }
}

