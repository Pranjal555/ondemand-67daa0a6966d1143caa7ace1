
const axios = require('axios');
const { Readable } = require('stream');

const API_KEY = '<replace_api_key>';
const EXTERNAL_USER_ID = '<replace_external_user_id>';

async function createChatSession() {
  const url = 'https://api-dev.on-demand.io/chat/v1/sessions';
  const headers = { apikey: API_KEY };
  const body = {
    agentIds: [],
    externalUserId: EXTERNAL_USER_ID,
  };

  const response = await axios.post(url, body, { headers });
  if (response.status !== 201) {
    throw new Error('Failed to create chat session');
  }
  return response.data.data.id;
}

async function submitQuery(sessionId, query) {
  const url = `https://api-dev.on-demand.io/chat/v1/sessions/${sessionId}/query`;
  const headers = { apikey: API_KEY };
  const body = {
    endpointId: 'predefined-openai-gpt4o',
    query: query,
    agentIds: [
      'plugin-1712327325',
      'plugin-1713962163',
      'plugin-1713954536',
      'plugin-1713958591',
      'plugin-1713958830',
      'plugin-1713961903',
      'plugin-1713967141',
    ],
    responseMode: 'sync',
    reasoningMode: 'medium',
  };

  const response = await axios.post(url, body, { headers });
  if (response.status !== 200) {
    throw new Error('Failed to submit query');
  }
  return response.data;
}

(async () => {
  try {
    const sessionId = await createChatSession();
    const result = await submitQuery(sessionId, 'Put your query here');
    console.log(result);
  } catch (err) {
    console.error(err);
  }
})();
