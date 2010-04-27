
/* ADC INTERRUPT */

// Unbuffered system timer, as ADC interrupt is assumed to be non-reentrant.
SystemTimer_t raw_systemTimer = 0;
// ADC interrupt
SIGNAL(ADC_vect){
// ADC interrupt feeds the system timer
  raw_systemTimer++;
  systemTimer.sinkData(raw_systemTimer);
}

void init_ADC(){
// Enable ADC
  sbi(ADCSRA, ADEN);
// Free-running
  sbi(ADCSRA, ADATE);
  // Trigger source: Overflow
  ADCSRB = 0;
// Interrupts
  sbi(ADCSRA, ADIE);
// Prescale by 128 (max)
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
// Right-adjusted values.
  cbi(ADMUX, ADLAR);

// PC0 needs to be an input for chan 1; PC1 for chan 2.
  cbi(DDRC, PC0);
  cbi(DDRC, PC1);

// First read is from channel 0, Aref reference.
  ADMUX = 0;
// Activate interrupts.
  sei();
// Start first conversion.
  sbi(ADCSRA, ADSC);
}

