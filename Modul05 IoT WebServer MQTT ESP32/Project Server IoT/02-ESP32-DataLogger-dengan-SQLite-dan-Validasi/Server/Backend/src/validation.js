'use strict';

const { z } = require('zod');

// ── Zod schema untuk data sensor ──────────────────────────────
const sensorSchema = z.object({
  suhu: z
    .number({ invalid_type_error: 'suhu harus berupa angka' })
    .min(-10, 'suhu minimum adalah -10°C')
    .max(60,  'suhu maksimum adalah 60°C'),

  kelembaban: z
    .number({ invalid_type_error: 'kelembaban harus berupa angka' })
    .min(0,   'kelembaban minimum adalah 0%')
    .max(100, 'kelembaban maksimum adalah 100%'),

  cahaya: z
    .number({ invalid_type_error: 'cahaya harus berupa angka' })
    .int('cahaya harus berupa bilangan bulat')
    .min(0,    'cahaya minimum adalah 0 (ADC)')
    .max(4095, 'cahaya maksimum adalah 4095 (12-bit ADC)'),

  device_id: z
    .string()
    .max(64, 'device_id maksimum 64 karakter')
    .optional()
    .default('esp32-01'),
});

/**
 * Validates incoming sensor payload.
 * @param {unknown} data  Raw request body
 * @returns {{ success: true, data: SensorData } | { success: false, error: string }}
 */
function validateSensorData(data) {
  const result = sensorSchema.safeParse(data);
  if (result.success) {
    return { success: true, data: result.data };
  }

  // Collect all Zod issue messages into a readable string
  const error = result.error.issues
    .map((issue) => `${issue.path.join('.') || 'root'}: ${issue.message}`)
    .join('; ');

  return { success: false, error };
}

module.exports = { validateSensorData, sensorSchema };
