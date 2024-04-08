import { initializeApp } from "firebase/app";
import {
  getFirestore,
  collection,
  getDocs,
  onSnapshot,
} from "firebase/firestore";

// Tu configuración de Firebase
const firebaseConfig = {
  apiKey: "AIzaSyD0iTuSuPCzE-SOTKwIupNgxJxD8NWQib0",
  authDomain: "project-iot-adc04.firebaseapp.com",
  projectId: "project-iot-adc04",
  storageBucket: "project-iot-adc04.appspot.com",
  messagingSenderId: "505111473711",
  appId: "1:505111473711:web:8a9b0accfb177999a2a0da",
  measurementId: "G-LHJE3PD5JM"
};

// Inicializa Firebase
const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

const lechugaRef = collection(db, "Lechuga");

// Obtener datos de la colección "Lechuga"
export const getLechugaData = async () => {
  const lechugaSnapshot = await getDocs(lechugaRef);
  const lechugaArray = lechugaSnapshot.docs.map((doc) => doc.data());
  const sortedData = lechugaArray.sort((a, b) => a.alive - b.alive);
  console.log(sortedData);
};

export const getLastLechugaData = async () => {
  const lechugaSnapshot = await getDocs(lechugaRef);
  const lechugaArray = lechugaSnapshot.docs.map((doc) => doc.data());
  const sortedData = lechugaArray.sort((a, b) => a.alive - b.alive);
  return sortedData[sortedData.length - 1];
};
